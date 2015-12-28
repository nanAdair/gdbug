/*
 * =====================================================================================
 *
 *       Filename:  instruction.h
 *
 *    Description:  classes of instruction
 *
 *        Version:  1.0
 *        Created:  12/14/2015 14:56:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef GDBUG_SCARAB_INSTRUCTION_H

#include <list>
using std::list;
#include <map>
using std::map;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <fstream>
using std::ostream;

#include <capstone/capstone.h>

#include "type.h"

class Section;
class SectionVec;
//class Operand;

class Instruction : public std::enable_shared_from_this<Instruction>
{
public:
    friend class InstrList;
    friend ostream &operator<<(ostream&, const Instruction&);
    Instruction();
    Instruction(const cs_insn&, shared_ptr<Section>);
    ~Instruction();

    UINT32 get_instruction_address() const;
    UINT8 *get_instruction_data() const 
    { return data_; }
private:
     //prefixes 
    //INT8 lock_and_repeat_;
    //INT8 segment_override_;
    //INT8 operand_size_override_;
    //INT8 address_size_override_;
    /* opcode */
    UINT32 opcode_;
    /* operands */
    //shared_ptr<Operand> dest;
    //shared_ptr<Operand> src1;
    //shared_ptr<Operand> src2;
    //shared_ptr<Operand> src3;
    /* used for address */
    //INT8 mod_;
    //INT8 rm_;
    //INT8 regop_;
    /* flags */
    UINT32 sec_type_;
    //INT8 s_;
    /* address */
    UINT32 origin_address_;
    UINT32 final_address_;
    /* instruction type */
    INT8 type_;
    /* instruction class */
    //INT8 class_;
    /* pwd that might be affected */
    //UINT32 pwd_affected_;
    /* pwd that used */
    //UINT32 pwd_used_;
    /* have ModR/M or not */
    //bool ModRM_;
    /* SIB */
    //INT8 SIB_;
    /* assembly in ascii */
    string assembly_;
    /* return machine code */
    //string ret_machine_code_;
    /* opcode in ascii */
    string mnemonic_;
    /* new CS and ESP(if existed) */
    //UINT16 new_cs_;
    //UINT32 new_eip_;
    /* instruction size */
    UINT32 size_;
    /* operand size */
    //UINT32 handler_index_;
    /* binary data */
    UINT8 *data_;
    /* next instruction in physical */
    shared_ptr<Instruction> next_;
};

class InstrList
{
public:
    friend ostream &operator<<(ostream&, const InstrList&);
    InstrList() {}
    InstrList(const SectionVec&);

    shared_ptr<Instruction> get_instr_by_address(UINT32) const;
private:
    list<shared_ptr<Instruction> > instr_list_;
    map<UINT32, shared_ptr<Instruction> > addr_to_instr_map_;
    UINT32 begin_addr_;
    UINT32 end_addr_;
};
#endif
