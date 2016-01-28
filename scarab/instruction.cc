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
#include "edge.h"
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

INT32 SCInstr::get_instruction_size() const
{
     return size;
}
INT32 SCInstr::get_opcode() const
{
    return opcode;
}

Operand* SCInstr::get_dest() const
{
    return dest;
}

shared_ptr<SCInstr> SCInstr::get_jump_target() const
{
    return jump_target_;
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

void SCInstr::set_jump_target(shared_ptr<SCInstr> instr)
{
    jump_target_ = instr;
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

void SCInstr::toASMInstruction(ASMINSTRUCTION* asmInstruction){
    asmInstruction->lockAndRepeat_ = this->lockAndRepeat;
    asmInstruction->segmentOverride_ = this->segmentOverride;
    if (this->OperandSizeOverride == 0x66){
	asmInstruction->OperandSizeOverride_ = 1;
    }
    if (this->AddressSizeOverride == 0x67){
	asmInstruction->AddressSizeOverride_ = 1;
    }
    asmInstruction->waitPrefix_ = -1;

    asmInstruction->dest_ = operand2ASMOperand(this->dest);
    asmInstruction->src1_ = operand2ASMOperand(this->src1);
    asmInstruction->src2_ = operand2ASMOperand(this->src2);
    asmInstruction->src3_ = operand2ASMOperand(this->src3);

    asmInstruction->type_ = this->type;
    asmInstruction->ModRM_ = this->ModRM;
    asmInstruction->s_ = this->s;
    asmInstruction->opcode_ = this->opcode;
    asmInstruction->auxiliaryOpcode_ = -1;
    asmInstruction->mnemonic_ = this->mnemonic;
    asmInstruction->instrstr_index_ = 0;
    asmInstruction->address_ = this->address;
    asmInstruction->alternativeOpcode_ = -1;
}

ASMOPERAND* SCInstr::operand2ASMOperand(Operand *operand){
    if (operand == NULL || operand->isDefault){
	return NULL;
    }

    ASMOPERAND* asmOperand = new ASMOPERAND();

    asmOperand->type_ = operand->type;
    asmOperand->operand_ = operand->operand;
    asmOperand->operand_size_ = operand->operand_size;
    asmOperand->segment_ = operand->segment;
    asmOperand->scale_ = operand->scale;
    asmOperand->index_ = operand->index;
    asmOperand->base_ = operand->base;
    asmOperand->displacement_ = operand->displacement;
    asmOperand->displacement_size_ = operand->displacement_size;
    asmOperand->addressing_size_ = operand->addressing_size;
    asmOperand->default_ = operand->isDefault;
    asmOperand->signExtended_ = false;

    return asmOperand;
}

unsigned char* SCInstr::instruction2Binary(){
    ASMINSTRUCTION* asmInstruction = new ASMINSTRUCTION();
    toASMInstruction(asmInstruction);

    UINT8 modrm = (this->mod << 6) | (this->regop << 3) | (this->rm);

    Semantic semantic;
    semantic.encodeInstr(asmInstruction, modrm);

    char* machineCode = new char[20], *singleInstrMachineCode;
    machineCode[0] = '\0';

    for (int i = 0; i < (int)asmInstruction->size_; i++){
	singleInstrMachineCode = int2str(&asmInstruction->machineCode_[i], sizeof(UINT8), 1, 0, false);
	strcat(machineCode, singleInstrMachineCode);
	free(singleInstrMachineCode);
    }

    Disasm disasm;
    disasm.disassembleMachineCode(machineCode, int2str(&asmInstruction->address_, sizeof(UINT32), 1, 0, false));

    printInstrDetail();

    // don't forget to delete
    return asmInstruction->machineCode_;
}

void SCInstr::printInstrDetail(){
    cout << "Address: " << int2str((void*)&this->address, sizeof(UINT32), 1, 0, false) << endl;

    cout << "Instruction type: ";
    switch (this->type){
    case UNDEFINED_INSTRUCTION:
	cout << "UNDEFINED_INSTRUCTION" << endl;
	break;
    case NORMAL_INSTRUCTION:
	cout << "NORMAL_INSTRUCTION" << endl;
	break;
    case STACK_OPERATE_INSTRUCTION:
	cout << "STACK_OPERATE_INSTRUCTION" << endl;
	break;
    case FLOW_INSTRUCTION:
	cout << "FLOW_INSTRUCTION" << endl;
	break;
    case IRREPLACEABLE_INSTRUCTION:
	cout << "IRREPLACEABLE_INSTRUCTION" << endl;
	break;
    case FLOAT_INSTRUCTION:
	cout << "FLOAT_INSTRUCTION" << endl;
	break;
    case PREFIX_INSTRUCTION:
	cout << "PREFIX_INSTRUCTION" << endl;
	break;
    case SSE1_INSTRUCTION:
	cout << "SSE1_INSTRUCTION" << endl;
	break;
    case SSE2_INSTRUCTION:
	cout << "SSE2_INSTRUCTION" << endl;
	break;
    case SSE3_INSTRUCTION:
	cout << "SSE3_INSTRUCTION" << endl;
	break;
    case SSSE3_INSTRUCTION:
	cout << "SSSE3_INSTRUCTION" << endl;
	break;
    case SSE41_INSTRUCTION:
	cout << "SSE41_INSTRUCTION" << endl;
	break;
    case SSE42_INSTRUCTION:
	cout << "SSE42_INSTRUCTION" << endl;
	break;
    case MMX_INSTRUCTION:
	cout << "MMX_INSTRUCTION" << endl;
	break;
    case VMX_INSTRUCTION:
	cout << "VMX_INSTRUCTION" << endl;
	break;
    case SMX_INSTRUCTION:
	cout << "SMX_INSTRUCTION" << endl;
	break;
    case NEED_FURTHER_PARSED:
	cout << "NEED_FURTHER_PARSED" << endl;
	break;
    case SPECIAL_MNEMONIC:
	cout << "SPECIAL_MNEMONIC" << endl;
	break;
    default:
	cout << "UNKNOWN INSTRUCTION!!!!!" << endl;
	break;
    }

    cout << "Lock And Repeat: ";
    switch ((UINT8)this->lockAndRepeat){
    case PREFIX_LOCK:
	cout << "PREFIX_LOCK" << endl;
	break;
    case PREFIX_REPN:
	cout << "PREFIX_REPN" << endl;
	break;
    case PREFIX_REP:
	cout << "PREFIX_REP" << endl;
	break;
    default:
	cout << (INT32)lockAndRepeat << endl;
	break;
    }

    cout << "Segment override: ";
    switch (this->segmentOverride){
    case ES:
	cout << "ES" << endl;
	break;
    case CS:
	cout << "ES" << endl;
	break;
    case SS:
	cout << "ES" << endl;
	break;
    case DS:
	cout << "ES" << endl;
	break;
    case FS:
	cout << "ES" << endl;
	break;
    case GS:
	cout << "ES" << endl;
	break;
    default:
	cout << (INT32)this->segmentOverride << endl;
	break;
    }

    cout << "Operand size override: " << (INT32)this->OperandSizeOverride << endl;
    cout << "Address size override: " << (INT32)this->AddressSizeOverride << endl;

    cout << "Mnemonic: " << this->mnemonic << endl;
    cout << "Opcode: " << int2str((void*)&this->opcode, sizeof(UINT32), 1, 0, false) << endl;

    if (this->ModRM){
	cout << "MOD: " << int2str((void*)&this->mod, sizeof(UINT8), 1, 0, false) << endl;
	cout << "RM: " << int2str((void*)&this->rm, sizeof(UINT8), 1, 0, false) << endl;
	cout << "REGOP: " << int2str((void*)&this->regop, sizeof(UINT8), 1, 0, false) << endl;
    }

    cout << endl;
    if (this->dest){
	cout << "Dest:" << endl;
	this->dest->printOperandDetail();
	cout << endl;
    }
    if (this->src1){
	cout << "Src1:" << endl;
	this->src1->printOperandDetail();
	cout << endl;
    }
    if (this->src2){
	cout << "Src2:" << endl;
	this->src2->printOperandDetail();
	cout << endl;
    }
    if (this->src3){
	cout << "Src3:" << endl;
	this->src3->printOperandDetail();
	cout << endl;
    }

    cout << "NEW EIP: " << int2str((void*)&this->new_eip, sizeof(UINT32), 1, 0, false) << endl;
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
		memcpy(buffer, data + start, size);
		//printf("Machine Code: %s\n", int2str(&buffer, sizeof(UINT32), 1, 0, false));
		ret = disasm.disassembler((INT8*)buffer, size, address, 0, instr);
		if (ret == NOT_ENOUGH_CODE){
		    //printf("NOT ENOUGH CODE\n");
		    start++;
		    address++;
		    continue;
		    //break;
		}
		if (start + ret > data_size){
		    break;
		}
		//printf("%s   %s\n", instr->ret_machineCode, instr->assembly);

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

shared_ptr<SCInstr> InstrList::get_instr_by_exact_address(UINT32 addr) const
{
    shared_ptr<SCInstr> res;
    map<UINT32, shared_ptr<SCInstr> >::const_iterator it = addr_to_instr_map_.find(addr);
    if (it != addr_to_instr_map_.end())
	res = it->second;
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

void InstrList::resolve_function_exit_block()
{
    report(RL_FOUR, "resolve ret block to exit block");
    InstrIterT it;
    for (it = instr_list_.begin(); it != instr_list_.end(); it++) {
	if ((*it)->isReturnClass()) {
	    shared_ptr<Block> from, to;
	    from = (*it)->get_block();
	    to = INSTR_FUNCTION(*it)->get_exit_block();
	    EDGELIST->add_bbl_edge(from, to, ET_EXIT);
	}
    }
}

UINT32 SCInstr::get_target_address()
{
    UINT32 res = 0;
    if (!isConditionalJmpClass() && !isCallClass() && !isJmpClass())
	return res;
    if (dest->type != OPERAND_FLOW)
	return res;
    // absolute call/jmp
    if (opcode == 0x91 || opcode == 0xea)
	return dest->getOperand();

    // relative
    INT32 rel = dest->getOperand();
    res = get_instruction_address() + size + rel;
    return res;
}

INT32 InstrList::update_pc_relative_jumps()
{
    report(RL_FOUR, "fix pc relative jumps instructions");
    int change = 0;
    // TODO: use jump_target_ to record target instr for now
    // cause target is not the first instruction of target block
    for (InstrIterT it = instr_list_.begin(); it != instr_list_.end(); it++) {
	if (!(*it)->isConditionalJmpClass() && !(*it)->isCallClass() && !(*it)->isJmpClass())
	    continue;
	if ((*it)->dest->type != OPERAND_FLOW)
	    continue;
	if ((*it)->isConditionalJmpClass()) {
	    UINT32 target_addr = (*it)->get_jump_target()->get_instruction_address();
	    INT32 displacement = target_addr - ((*it)->get_instruction_address() + (*it)->get_instruction_size());
	    if (displacement != (*it)->get_dest()->getOperand()) {
		change++;
		// TODO: how to update instruction binary
		// (*it)->update_instruction_binary(displacement);
	    }
	    continue;
	}
	if ((*it)->isJmpClass()) {
	    UINT32 target_addr = (*it)->get_jump_target()->get_instruction_address();
	    INT32 displacement;
	    if ((*it)->get_opcode() != 0xea) {
		displacement = target_addr - ((*it)->get_instruction_address() + (*it)->get_instruction_size());
	    }
	    else
		displacement = target_addr;
	    if (displacement != (*it)->get_dest()->getOperand()) {
		change++;
		// TODO: how to update instruction binary
		// (*it)->update_instruction_binary(displacement);
	    }
	    continue;
	}
	if ((*it)->isCallClass()) {
	    UINT32 target_addr = (*it)->get_jump_target()->get_instruction_address();
	    INT32 displacement;
	    if ((*it)->get_opcode() != 0x91) {
		displacement = target_addr - ((*it)->get_instruction_address() + (*it)->get_instruction_size());
	    }
	    else
		displacement = target_addr;
	    if (displacement != (*it)->get_dest()->getOperand()) {
		change++;
		// TODO: how to update instruction binary
		// (*it)->update_instruction_binary(displacement);
	    }
	    continue;
	}
    }
    return change;
}

void InstrList::resolve_targets()
{
    report(RL_FOUR, "resolve cfg targets");
    for (InstrIterT it = instr_list_.begin(); it != instr_list_.end(); it++) {
	shared_ptr<Block> from = (*it)->get_block();
	shared_ptr<Block> to = BLOCKLIST->get_next_block(from);
	// 1: Normal instr
	if (!(*it)->isPCChangingClass()) {
	    if ((*it)->has_flag(BBL_END) &&
		!(EDGELIST->edge_exist_or_not(from, to))) {
		if (!to) {
		    // last block in the program
		    EDGELIST->add_bbl_edge(from, BLOCKLIST->HELL, ET_HELL);
		}
		else {
		    EDGELIST->add_bbl_edge(from, to, ET_NORMAL);
		}
		from->set_type(BT_NORMAL);
	    }
	    continue;
	}
	// 2. Conditional branch instr
	if ((*it)->isConditionalJmpClass()) {
	    if (to) {
		EDGELIST->add_bbl_edge(from, to, ET_FALSE);
	    }
	    UINT32 target_addr;
	    // target is through indirect calculation
	    if ((target_addr = (*it)->get_target_address()) == 0)
		continue;
	    shared_ptr<INSTRUCTION> to_instr = this->get_instr_by_exact_address(target_addr);
	    //cout << **it;
	    //cout << target_addr << endl;
	    if (to_instr)
		EDGELIST->add_bbl_edge(from, to_instr->get_block(), ET_TRUE);
	    (*it)->set_jump_target(to_instr);
	    continue;
	}
	// 3. Jmp instr
	if ((*it)->isJmpClass()) {
	    UINT32 target_addr;
	    // target is through indirect calculation
	    if ((target_addr = (*it)->get_target_address()) == 0)
		continue;
	    shared_ptr<INSTRUCTION> to_instr = this->get_instr_by_exact_address(target_addr);
	    //cout << **it;
	    //cout << target_addr << endl;
	    if (to_instr) {
		EDGELIST->add_bbl_edge(from, to_instr->get_block(), ET_UNCOND);
	    }
	    (*it)->set_jump_target(to_instr);
	    continue;
	}
	// 4. Call instr
	if ((*it)->isCallClass()) {
	    UINT32 target_addr;
	    // target is through indirect calculation
	    if ((target_addr = (*it)->get_target_address()) == 0)
		continue;
	    shared_ptr<INSTRUCTION> to_instr = this->get_instr_by_exact_address(target_addr);
	    //cout << **it;
	    //cout << target_addr << endl;
	    if (to_instr) {
		EDGELIST->add_bbl_edge(from, to_instr->get_block(), ET_FUNCALL);
		if (to) {
		    EDGELIST->add_bbl_edge(to_instr->get_block(), to, ET_RETURN);
		}
	    }
	    (*it)->set_jump_target(to_instr);
	    if (to) {
		EDGELIST->add_bbl_edge(from, to, ET_FUNLINK);
	    }
	    continue;
	}
	// 5. Ret instr added above
	if ((*it)->isReturnClass()) {
	    (*it)->get_block()->set_type(BT_RETURN);
	    continue;
	}
    }
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
    this->resolve_function_exit_block();
    this->resolve_targets();
    FUNLIST->resolve_entryless_function();
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
    //os << byte_to_str(ins.binary, ins.size) << "\t";
    os << ins.ret_machineCode << "\t";
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
