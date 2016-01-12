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
class Operand;

class SCInstr;
typedef SCInstr INSTRUCTION;

class SCInstr 
{
public:
    friend class InstrList;
    friend ostream &operator<<(ostream&, const INSTRUCTION&);
    SCInstr();
    SCInstr(struct SCINSTR_INTERNAL_STRUCT tmp);
    ~SCInstr();

    // ==== getters and setters ====
    UINT32 get_instruction_address() const;
    UINT8* get_instruction_data() const;
    //void setFlag(IFLAG flag);
    //bool hasFlag(IFLAG flag);
    //void removeFlag(IFLAG flag);
    
    //void setBlock(SCBlock* bbl);

    //SCBlock* getBlock();
    //UINT32 getAddr();
    //Operand* getDest();

    // ==== methods ====
    //bool isPCChangingClass();
    //bool isReturnClass();
    //bool isCallClass();
    //bool isJmpClass();
    //bool isConditionalJmpClass();
    //bool isLoopClass();
    //bool isMovClass();
    //bool isNOPClass();
    //bool isPopClass();
    //bool isPushClass();
    //bool isSubClass();
    
    //bool isDataInstruction();

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
    //SCBlock* i_block;
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
    INSTRUCTION *next;
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
    INSTRUCTION *next;
    //struct _INSTRUCTION *next;
};

class InstrList
{
public:
    friend ostream &operator<<(ostream&, const InstrList&);
    static InstrList* sharedInstrList();
    static InstrList* shared_instr_list_;

    InstrListT get_instr_list();

    void disassemble(const SectionVec&);
    void disassemble2(const SectionVec&);
    shared_ptr<SCInstr> get_instr_by_address(UINT32) const;
    void update_sections_size(SectionVec&) const;
    void update_sections_data(SectionVec&) const;
    void update_instr_address(SectionVec&);

    //void construct_cfg();

private:
    InstrList(){}
    InstrListT instr_list_;
    map<UINT32, shared_ptr<SCInstr> > addr_to_instr_map_;
    UINT32 begin_addr_;
    UINT32 end_addr_;
};
#endif
