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

#include "log.h"
#include "section.h"
using namespace std;

Instruction::Instruction():
    opcode_(0),
    sec_type_(0),
    origin_address_(0),
    final_address_(0),
    type_(0),
    size_(0),
    data_(NULL)
{}

Instruction::Instruction(const cs_insn &instr, shared_ptr<Section> sec): Instruction()
{
    cs_x86 *x86;
    if (instr.detail == NULL)
        return;
    x86 = &((instr.detail)->x86);
    opcode_ = *(x86->opcode);
    mnemonic_ = string(instr.mnemonic);

    assembly_ = mnemonic_ + " " + string(instr.op_str);
    origin_address_ = instr.address;
    size_ = instr.size;

    // ======fileds get from related section=======
    data_ = new UINT8 [size_];
    UINT8 *sec_data = sec->get_section_data();
    UINT32 offset = origin_address_ - sec->get_section_address();
    memcpy(data_, sec_data+offset, size_);

    string sec_name = sec->get_section_name();
    if (sec_name == ".init")
        sec_type_ = SECTION_INIT;
    else if (sec_name == ".fini")
        sec_type_ = SECTION_FINI;
    else if (sec_name == ".text")
        sec_type_ = SECTION_TEXT;
    else if (sec_name == ".plt")
        sec_type_ = SECTION_PLT;
    else
        sec_type_ = SECTION_OTHER;
}

UINT32 Instruction::get_instruction_address() const
{
    return final_address_ ? final_address_ : origin_address_;
}

Instruction::~Instruction()
{
    if (data_) {
        delete [] data_;
        data_ = NULL;
        size_ = 0;
    }
}

// ===== InstrList ======
InstrList* InstrList::shared_instr_list_ = NULL;

InstrList* InstrList::sharedInstrList()
{
    if (shared_instr_list_ == NULL)
        shared_instr_list_ = new InstrList();
    return shared_instr_list_;
}

//InstrList::InstrList(const SectionVec &obj_sec_vec)
void InstrList::disassemble(const SectionVec &obj_sec_vec)
{
    report(RL_THREE, "disassemble binary code begin");
    UINT32 number = obj_sec_vec.get_section_vec_size();
    csh handle;
    cs_insn *insn;

	if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle)) 
		report(RL_ONE, "ERROR: Failed to initialize engine!");
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    for (UINT32 i = 1; i < number; i++) {
        shared_ptr<Section> cur_sec = obj_sec_vec.get_section_by_index(i);
        UINT32 flags = cur_sec->get_section_flags();
        if (flags & SHF_EXECINSTR) {
            int count = cs_disasm(handle, cur_sec->get_section_data(), cur_sec->get_section_size(), cur_sec->get_section_address(), 0, &insn);
            if (count) {
                for (int j = 0; j < count; j++) {
                    shared_ptr<Instruction> cur_instr = make_shared<Instruction>(insn[j], cur_sec);
                    if (instr_list_.size() > 0) {
                        shared_ptr<Instruction> last = instr_list_.back();
                        last->next_ = cur_instr;
                    }
                    instr_list_.push_back(cur_instr);
                    addr_to_instr_map_.insert(pair<UINT32, shared_ptr<Instruction> >(cur_instr->origin_address_, cur_instr));
                }
                cs_free(insn, count);
            }
            else 
                report(RL_ONE, "Failed to disassemble given code!");
        }
    }

    begin_addr_ = instr_list_.front()->get_instruction_address();
    end_addr_ = instr_list_.back()->get_instruction_address() + instr_list_.back()->size_;

    cs_close(&handle);
    report(RL_THREE, "disassemble binary code end");
}

shared_ptr<Instruction> InstrList::get_instr_by_address(UINT32 addr) const
{
    shared_ptr<Instruction> res;
    UINT32 start_addr = 0x8048000;
    while (addr >= begin_addr_ && addr < end_addr_) {
        map<UINT32, shared_ptr<Instruction> >::const_iterator it = addr_to_instr_map_.find(addr);
        if (it != addr_to_instr_map_.end()) {
            res = it->second;
            break;
        }
        addr -= 1;
    }
    return res;
}

void InstrList::update_sections_size(SectionVec &obj_sec_vec) const
{
    report(RL_FOUR, "update sections size");
    list<shared_ptr<Instruction> >::const_iterator it = instr_list_.begin();
    int datasize = 0;
    UINT32 last_sec, cur_sec;
    last_sec = cur_sec = (*it)->sec_type_;
    shared_ptr<Section> sec;

    for(; it != instr_list_.end(); it++) {
        cur_sec = (*it)->sec_type_;
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
        datasize += (*it)->size_;
    }
}

void InstrList::update_sections_data(SectionVec &obj_sec_vec) const
{
    report(RL_FOUR, "update sections data");
    list<shared_ptr<Instruction> >::const_iterator it = instr_list_.begin();
    int offset = 0;
    int last_sec, cur_sec;
    last_sec = -1;
    shared_ptr<Section> sec;

    for(; it != instr_list_.end(); it++) {
        cur_sec = (*it)->sec_type_;
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

        sec->expand_section_data((*it)->data_, (*it)->size_, 1);
    }
}

void InstrList::update_instr_address(SectionVec &obj_sec_vec)
{
    report(RL_FOUR, "update instructions address");
    list<shared_ptr<Instruction> >::iterator it = instr_list_.begin();
    int offset = 0;
    int last_sec, cur_sec;
    last_sec = -1;
    shared_ptr<Section> sec;

    for(; it != instr_list_.end(); it++) {
        cur_sec = (*it)->sec_type_;
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

        (*it)->final_address_ = sec->get_section_address() + offset;
        offset += (*it)->size_;
    }
    // no need to update addr_to_instr_map_, won't use this
    
    end_addr_ = instr_list_.back()->get_instruction_address() + instr_list_.back()->size_;
}

//void InstrList::construct_cfg()
//{
    //BLOCKLIST->mark_bbl();
//}

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

ostream& operator<<(ostream &os, const Instruction &ins)
{
    //os << hex << ins.origin_address_ << "\t";
    os << hex << ins.get_instruction_address() << "\t";
    os << byte_to_str(ins.data_, ins.size_) << "\t";
    os << ins.assembly_;
    os << endl;
    return os;
}

ostream& operator<<(ostream &os, const InstrList &instr_list)
{
    list<shared_ptr<Instruction> >::const_iterator it;
    for (it = instr_list.instr_list_.begin(); it != instr_list.instr_list_.end(); it++)
        os << **it;

    return os;
}

