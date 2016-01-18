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
#define GDBUG_SCARAB_INSTRUCTION_H

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
class SymbolVec;
class Operand;
class Block;

class SCInstr;
typedef SCInstr INSTRUCTION;

class SCInstr 
{
public:
    friend class InstrList;
    friend ostream &operator<<(ostream&, const SCInstr&);
    SCInstr();
    SCInstr(struct SCINSTR_INTERNAL_STRUCT tmp);
    ~SCInstr();

    // ==== getters and setters ====
    UINT32 get_instruction_address() const;
    UINT8* get_instruction_data() const;
    INT32 get_instruction_size() const;
    INT32 get_opcode() const;
    Operand* get_dest() const;
    shared_ptr<SCInstr> get_jump_target() const;
    void set_flag(IFLAG flag);
    bool has_flag(IFLAG flag);
    void remove_flag(IFLAG flag);
    
    void set_block(shared_ptr<Block> bbl);
    void set_jump_target(shared_ptr<SCInstr> instr);
    shared_ptr<Block> get_block();
    //Operand* getDest();

    // ==== methods ====
    bool isPCChangingClass();
    bool isReturnClass();
    bool isCallClass();
    bool isJmpClass();
    bool isConditionalJmpClass();
    bool isLoopClass();
    bool isMovClass();
    bool isNOPClass();
    bool isPopClass();
    bool isPushClass();
    bool isSubClass();
    
    bool isDataInstruction();

    UINT32 get_target_address();

    //bool isOnlyInstrInBBL();

    /* prefixes */
    INT8 lockAndRepeat;
    INT8 segmentOverride;
    INT8 OperandSizeOverride;
    INT8 AddressSizeOverride;
    /* operands */
    Operand *dest;
    Operand *src1;
    Operand *src2;
    Operand *src3;
    /* used for build cfg */
    UINT16 i_flags;
    shared_ptr<Block> i_block;
    /* use for addressing */
    INT8 mod;
    INT8 rm;
    INT8 regop;
    /* flags */
    INT32 secType;
    INT8 s;        // sign or not
    /* address */
    INT32 address;
    INT32 final_address;
    /* instruction type */
    INT8 type;
    /* instruction class */
    int instr_class;
    /* pwd that might be affected */
    INT32 pwd_affected;
    /* pwd that used */
    INT32 pwd_used;
    /* opcode */
    INT32 opcode;
    /* have ModR/M or not */
    bool ModRM;
    /* SIB */
    INT8 SIB;
    /* assembly */
    char *assembly;
    /* return machine code */
    char *ret_machineCode;
    /* mnemonic */
    const char *mnemonic;
    /* new CS and ESP(if existed) */
    INT16 new_cs;
    INT32 new_eip;
    /* size */
    INT32 size;
    /* operand size */
    int handlerIndex;
    /* binary */
    UINT8 *binary;
    /* next instruction */
    SCInstr *next;
    shared_ptr<SCInstr> jump_target_;
    //struct _INSTRUCTION *next;
};

// ==== INTERNAL ====
struct SCINSTR_INTERNAL_STRUCT {
    /* prefixes */
    INT8 lockAndRepeat;
    INT8 segmentOverride;
    INT8 OperandSizeOverride;
    INT8 AddressSizeOverride;
    /* operands */
    Operand *dest;
    Operand *src1;
    Operand *src2;
    Operand *src3;
    /* use for addressing */
    INT8 mod;
    INT8 rm;
    INT8 regop;
    /* flags */
    INT32 secType;
    INT8 s;        // sign or not
    /* address */
    INT32 address;
    INT32 final_address;
    /* instruction type */
    INT8 type;
    /* instruction class */
    int instr_class;
    /* pwd that might be affected */
    INT32 pwd_affected;
    /* pwd that used */
    INT32 pwd_used;
    /* opcode */
    INT32 opcode;
    /* have ModR/M or not */
    bool ModRM;
    /* SIB */
    INT8 SIB;
    /* assembly */
    char *assembly;
    /* return machine code */
    char *ret_machineCode;
    /* mnemonic */
    const char *mnemonic;
    /* new CS and ESP(if existed) */
    INT16 new_cs;
    INT32 new_eip;
    /* size */
    INT32 size;
    /* operand size */
    int handlerIndex;
    /* binary */
    UINT8 *binary;
    /* next instruction */
    SCInstr *next;
};

class InstrList
{
public:
    friend ostream &operator<<(ostream&, const InstrList&);
    static InstrList* sharedInstrList();
    static InstrList* shared_instr_list_;

    InstrListT get_instr_list();

    void disassemble2(const SectionVec&);

    shared_ptr<SCInstr> get_instr_by_address(UINT32) const;
    shared_ptr<SCInstr> get_instr_by_exact_address(UINT32 addr) const;
    shared_ptr<SCInstr> get_prev_instr(shared_ptr<SCInstr>);
    shared_ptr<SCInstr> get_next_instr(shared_ptr<SCInstr>);

    void update_sections_size(SectionVec&) const;
    void update_sections_data(SectionVec&) const;
    void update_instr_address(SectionVec&);

    void construct_cfg(const SymbolVec&);
    void resolve_function_exit_block();
    void resolve_targets();

    INT32 update_pc_relative_jumps(); 

private:
    InstrList(){}
    InstrListT instr_list_;
    //list<shared_ptr<SCInstr> > instr_list_;
    map<UINT32, shared_ptr<SCInstr> > addr_to_instr_map_;
    UINT32 begin_addr_;
    UINT32 end_addr_;
};
#endif
