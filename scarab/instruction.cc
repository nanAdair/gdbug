/*
 * =====================================================================================
 *
 *       Filename:  instruction.cc
 *
 *    Description:  class implementation of instruction
 *
 *        Version:  1.0
 *        Created:  12/14/2015 16:07:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "instruction.h"

#include <capstone/capstone.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "log.h"
#include "section.h"
#include "symbol.h"
#include "disasm.h"
#include "block.h"
#include "function.h"
using namespace std;

// ===== SCInstr ==========
SCInstr::SCInstr() :
    dest(NULL),
    src1(NULL),
    src2(NULL),
    src3(NULL),
    assembly(NULL),
    ret_machineCode(NULL),
    mnemonic(NULL),
    binary(NULL) {}

SCInstr::SCInstr(SCINSTR_INTERNAL_STRUCT tmp) 
{

   i_flags = 0;
   
   this->lockAndRepeat = tmp.lockAndRepeat;
   this->segmentOverride = tmp.segmentOverride;
   this->OperandSizeOverride = tmp.OperandSizeOverride;
   this->AddressSizeOverride = tmp.AddressSizeOverride;

   this->dest = tmp.dest;
   this->src1 = tmp.src1;
   this->src2 = tmp.src2;
   this->src3 = tmp.src3;

   this->mod = tmp.mod;
   this->rm = tmp.rm;
   this->regop = tmp.regop;

   this->address = tmp.address;
   this->final_address = tmp.final_address;

   this->type = tmp.type;
   this->instr_class = tmp.instr_class;
   this->pwd_affected = tmp.pwd_affected;
   this->pwd_used = tmp.pwd_used;
   this->opcode = tmp.opcode;
   this->ModRM = tmp.ModRM;
   this->SIB = tmp.SIB;
   this->assembly = tmp.assembly;
   this->ret_machineCode = tmp.ret_machineCode;
   this->mnemonic = tmp.mnemonic;

   this->new_cs = tmp.new_cs;
   this->new_eip = tmp.new_eip;
   this->size = tmp.size;
   this->handlerIndex = tmp.handlerIndex;
   this->binary = tmp.binary;
   this->next = tmp.next;
}

SCInstr::~SCInstr()
{
    //if (dest) {
        //free(dest);
        //dest = NULL;
    //}
    //if (src1) {
        //free(src1);
        //src1 = NULL;
    //}
    //if (src2) {
        //free(src2);
        //src2 = NULL;
    //}
    //if (src3) {
        //free(src3);
        //src3 = NULL;
    //}
    //free(assembly);
    //free(ret_machineCode);
    //free(binary);

    //assembly = NULL;
    //ret_machineCode = NULL;
    //binary = NULL;
}

// ===== SCInstr getter and setter =========

UINT32 SCInstr::get_instruction_address() const
{
    return final_address ? final_address : address;
}

UINT8* SCInstr::get_instruction_data() const 
{ 
    return binary; 
}

void SCInstr::set_flag(IFLAG flag) 
{
    (this->i_flags) |= flag;
}

void SCInstr::remove_flag(IFLAG flag) 
{
    (this->i_flags) &= (~flag);
}

bool SCInstr::has_flag(IFLAG flag) 
{
    return (bool)((this->i_flags) & flag);
}

void SCInstr::set_block(shared_ptr<Block> bbl)
{
    i_block = bbl;
}

shared_ptr<Block> SCInstr::get_block() {
    return this->i_block;
}

// ==== methods ====
bool SCInstr::isPCChangingClass() {
    return (type == FLOW_INSTRUCTION);
}

bool SCInstr::isCallClass() {
    return ((type==FLOW_INSTRUCTION) && (instr_class==CLASS_CALLF || instr_class==CLASS_CALL));
}

bool SCInstr::isReturnClass() {
    return ((type==FLOW_INSTRUCTION) && (instr_class==CLASS_RETN || instr_class==CLASS_RETF));
}

bool SCInstr::isJmpClass() {
    return ((type==FLOW_INSTRUCTION) && (instr_class==CLASS_JMP || instr_class==CLASS_JMPF || instr_class==CLASS_JMP_SHORT));
}

bool SCInstr::isConditionalJmpClass() {
    if (type == FLOW_INSTRUCTION) {
        if (instr_class<=CLASS_JG || instr_class==CLASS_JEXCZ)
            return true;
    }
    return false;
}

bool SCInstr::isLoopClass() {
    if (type == FLOW_INSTRUCTION) {
        if (instr_class==CLASS_LOOPDNE || instr_class==CLASS_LOOPDE || instr_class==CLASS_LOOPD)
            return true;
    }
    return false;
}

bool SCInstr::isMovClass() {
    return (type==NORMAL_INSTRUCTION && instr_class==CLASS_MOV);
}

bool SCInstr::isNOPClass() {
    return (type==IRREPLACEABLE_INSTRUCTION && instr_class==CLASS_NOP);
}

bool SCInstr::isPopClass() {
    return (type==STACK_OPERATE_INSTRUCTION && instr_class==CLASS_POP);
}

bool SCInstr::isPushClass() {
    return (type==STACK_OPERATE_INSTRUCTION && instr_class==CLASS_PUSH);
}

bool SCInstr::isSubClass() {
    return (type==NORMAL_INSTRUCTION && instr_class==CLASS_SUB);
}

bool SCInstr::isDataInstruction() {
    // currently no data instr
    return false;
}

// ===== InstrList ======
InstrList* InstrList::shared_instr_list_ = NULL;

InstrList* InstrList::sharedInstrList()
{
    if (shared_instr_list_ == NULL)
        shared_instr_list_ = new InstrList();
    return shared_instr_list_;
}

InstrListT InstrList::get_instr_list()
{
    return instr_list_;
}

void InstrList::disassemble2(const SectionVec &obj_sec_vec)
{
    report(RL_THREE, "disassemble binary code begin");
    UINT32 number = obj_sec_vec.get_section_vec_size();
    UINT8 buffer[MAX_INSTRUCTION_SIZE + 1];
    Disasm disasm;
    INT8 ret = -1;

    for (UINT32 i = 1; i < number; i++) {
        shared_ptr<Section> cur_sec = obj_sec_vec.get_section_by_index(i);
        UINT32 flags = cur_sec->get_section_flags();
        if (flags & SHF_EXECINSTR) {
            UINT8 *data = cur_sec->get_section_data();
            UINT32 size = MAX_INSTRUCTION_SIZE;
            UINT32 address = cur_sec->get_section_address();
            UINT32 data_size = cur_sec->get_section_size();
            UINT32 start = 0;

            while (1) {
                if (start >= data_size)
                    break;
                INSTRUCTION *instr = new INSTRUCTION();
                if (start + (INT32)MAX_INSTRUCTION_SIZE > data_size) {
                    size = data_size - start;
                }
                //if (start + (INT32)MAX_INSTRUCTION_SIZE > data_size) {
                    //size = data_size - start  + 1;
                    //if ((INT32)MAX_INSTRUCTION_SIZE > data_size)
                        //size -= 1;
                //}
                memcpy(buffer, data + start, size);
                ret = disasm.disassembler((INT8*)buffer, size, address, 0, instr);
                if (ret == NOT_ENOUGH_CODE)
                    break;

                string sec_name = cur_sec->get_section_name();
                if (sec_name == ".init")
                    instr->secType = SECTION_INIT;
                else if (sec_name == ".fini")
                    instr->secType = SECTION_FINI;
                else if (sec_name == ".text")
                    instr->secType = SECTION_TEXT;
                else if (sec_name == ".plt")
                    instr->secType = SECTION_PLT;
                else
                    instr->secType = SECTION_OTHER;

                shared_ptr<INSTRUCTION> cur_instr(instr);
                instr_list_.push_back(cur_instr);
                addr_to_instr_map_.insert(pair<UINT32, shared_ptr<INSTRUCTION> >(cur_instr->address, cur_instr));

                address += ret;
                start += ret;
            }
        }
    }

    begin_addr_ = instr_list_.front()->get_instruction_address();
    end_addr_ = instr_list_.back()->get_instruction_address() + instr_list_.back()->size;
    report(RL_THREE, "disassemble binary code end");
}

shared_ptr<SCInstr> InstrList::get_instr_by_address(UINT32 addr) const
{
    shared_ptr<SCInstr> res;
    UINT32 start_addr = 0x8048000;
    while (addr >= begin_addr_ && addr < end_addr_) {
        map<UINT32, shared_ptr<SCInstr> >::const_iterator it = addr_to_instr_map_.find(addr);
        if (it != addr_to_instr_map_.end()) {
            res = it->second;
            break;
        }
        addr -= 1;
    }
    return res;
}

shared_ptr<SCInstr> InstrList::get_prev_instr(shared_ptr<SCInstr> ins)
{
    shared_ptr<SCInstr> res;
    //InstrIterT it = find(instr_list_.begin(), instr_list_.end(), ins);
    InstrIterT it; 
    for (it = instr_list_.begin(); it != instr_list_.end(); it++) {
        if (*it == ins)
            break;
    }
    if (it == instr_list_.begin() || it == instr_list_.end())
        return res;
    res = *(--it);
    return res;
}

shared_ptr<SCInstr> InstrList::get_next_instr(shared_ptr<SCInstr> ins)
{
    shared_ptr<SCInstr> res;
    //InstrIterT it = find(instr_list_.begin(), instr_list_.end(), ins);
    InstrIterT it; 
    for (it = instr_list_.begin(); it != instr_list_.end(); it++) {
        if (*it == ins)
            break;
    }
    if (it == instr_list_.end() || ++it == instr_list_.end())
        return res;
    res = *it;
    return res;
}

void InstrList::update_sections_size(SectionVec &obj_sec_vec) const
{
    report(RL_FOUR, "update sections size");
    list<shared_ptr<INSTRUCTION> >::const_iterator it = instr_list_.begin();
    int datasize = 0;
    UINT32 last_sec, cur_sec;
    last_sec = cur_sec = (*it)->secType;
    shared_ptr<Section> sec;

    for(; it != instr_list_.end(); it++) {
        cur_sec = (*it)->secType;
        if (cur_sec != last_sec) {
            switch(last_sec) {
                case SECTION_INIT:
                    sec = obj_sec_vec.get_section_by_name(INIT_SECTION_NAME);
                    break;
                case SECTION_TEXT:
                    sec = obj_sec_vec.get_section_by_name(TEXT_SECTION_NAME);
                    break;
                case SECTION_FINI:
                    sec = obj_sec_vec.get_section_by_name(FINI_SECTION_NAME);
                    break;
                case SECTION_PLT:
                    sec = obj_sec_vec.get_section_by_name(PLT_SECTION_NAME);
                    break;
                default:
                    report(RL_ONE, "can't handle instr sec type for now");
                    exit(0);
            }

            sec->set_section_size(datasize);
            datasize = 0;
            last_sec = cur_sec;
        }
        datasize += (*it)->size;
    }
}

void InstrList::update_sections_data(SectionVec &obj_sec_vec) const
{
    report(RL_FOUR, "update sections data");
    list<shared_ptr<INSTRUCTION> >::const_iterator it = instr_list_.begin();
    int offset = 0;
    int last_sec, cur_sec;
    last_sec = -1;
    shared_ptr<Section> sec;

    for(; it != instr_list_.end(); it++) {
        cur_sec = (*it)->secType;
        if (cur_sec != last_sec) {
            switch(cur_sec) {
                case SECTION_INIT:
                    sec = obj_sec_vec.get_section_by_name(INIT_SECTION_NAME);
                    break;
                case SECTION_TEXT:
                    sec = obj_sec_vec.get_section_by_name(TEXT_SECTION_NAME);
                    break;
                case SECTION_FINI:
                    sec = obj_sec_vec.get_section_by_name(FINI_SECTION_NAME);
                    break;
                case SECTION_PLT:
                    sec = obj_sec_vec.get_section_by_name(PLT_SECTION_NAME);
                    break;
                default:
                    report(RL_ONE, "can't handle instr sec type for now");
                    exit(0);
            }

            last_sec = cur_sec;
            sec->clear_section_data();
        }

        sec->expand_section_data((*it)->binary, (*it)->size, 1);
    }
}

void InstrList::update_instr_address(SectionVec &obj_sec_vec)
{
    report(RL_FOUR, "update instructions address");
    list<shared_ptr<INSTRUCTION> >::iterator it = instr_list_.begin();
    int offset = 0;
    int last_sec, cur_sec;
    last_sec = -1;
    shared_ptr<Section> sec;

    for(; it != instr_list_.end(); it++) {
        cur_sec = (*it)->secType;
        if (cur_sec != last_sec) {
            switch(cur_sec) {
                case SECTION_INIT:
                    sec = obj_sec_vec.get_section_by_name(INIT_SECTION_NAME);
                    break;
                case SECTION_TEXT:
                    sec = obj_sec_vec.get_section_by_name(TEXT_SECTION_NAME);
                    break;
                case SECTION_FINI:
                    sec = obj_sec_vec.get_section_by_name(FINI_SECTION_NAME);
                    break;
                case SECTION_PLT:
                    sec = obj_sec_vec.get_section_by_name(PLT_SECTION_NAME);
                    break;
                default:
                    report(RL_ONE, "can't handle instr sec type for now");
                    exit(0);
            }

            last_sec = cur_sec;
            offset = 0;
        }

        (*it)->final_address = sec->get_section_address() + offset;
        offset += (*it)->size;
    }
     //no need to update addr_to_instr_map_, won't use this
    
    end_addr_ = instr_list_.back()->get_instruction_address() + instr_list_.back()->size;
}

void InstrList::construct_cfg(const SymbolVec &obj_sym_vec)
{
    report(RL_THREE, "construct control flow graph begin");
    BLOCKLIST->mark_bbl();
    FUNLIST->mark_functions(obj_sym_vec);
    BLOCKLIST->create_bbl();
    //cout << *BLOCKLIST << endl;
    FUNLIST->create_function_list(obj_sym_vec);
    //cout << *FUNLIST;
    report(RL_THREE, "construct control flow graph end");
}

/*-----------------------------------------------------------------------------
 *  helper printer functions below
 *-----------------------------------------------------------------------------*/
string byte_to_str(UINT8 *data, int length)
{
    stringstream ss;
    ss << hex;
    for (int i = 0; i < length; i++)
        ss << std::setw(2) << std::setfill('0') << (int)data[i];
    return ss.str();
}

ostream& operator<<(ostream &os, const INSTRUCTION &ins)
{
    os << hex << ins.address << "\t";
    os << ins.size << "\t";
    os << hex << ins.get_instruction_address() << "\t";
    os << byte_to_str(ins.binary, ins.size) << "\t";
    os << string(ins.assembly);
    os << endl;
    return os;
}

ostream& operator<<(ostream &os, const InstrList &instr_list)
{
    //list<shared_ptr<INSTRUCTION> >::const_iterator it;
    InstrListT::const_iterator it;
    for (it = instr_list.instr_list_.begin(); it != instr_list.instr_list_.end(); it++)
        os << **it;
    os << "begin address is " << instr_list.begin_addr_ << endl;
    os << "end address is " << instr_list.end_addr_ << endl;

    return os;
}

