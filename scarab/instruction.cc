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

Instruction::Instruction(const cs_insn &instr, shared_ptr<Section> sec)
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

InstrList::InstrList(const SectionVec &obj_sec_vec)
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

/*-----------------------------------------------------------------------------
 *  helper printer functions below
 *-----------------------------------------------------------------------------*/
ostream& operator<<(ostream &os, const Instruction &ins)
{
    os << hex << ins.origin_address_ << "\t";
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
