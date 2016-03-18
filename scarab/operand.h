#ifndef __Scarab__Operand__H__
#define __Scarab__Operand__H__

#include "type.h"
#include "dis-table.h"
#include <set>

class Operand{

 public:
    Operand();
    Operand(INT8, INT32, OPERAND_SIZE, OPERAND_TYPE, INT8, INT8,
	    INT8, INT32, int, OPERAND_SIZE, bool);
    Operand(INT8, INT32, OPERAND_SIZE, OPERAND_TYPE, INT8, INT8,
	    INT8, INT32, int, OPERAND_SIZE, int);

    Operand(int, int, OPERAND_SIZE, OPERAND_TYPE, int, int, int,
	    int, int, OPERAND_SIZE, int);
    Operand(int, HAVE_AUXILIARY_CODE, OPERAND_SIZE, OPERAND_TYPE, int, int, int,
	    int, int, OPERAND_SIZE, int);

    INT32 getOperand();
    void setOperand(INT32 reg, INT32 displacement=0);
    void printOperandDetail();
    bool addr_above_ebp();
    void accumulate_reg(set<int>&);
    void changed_to_sib(int);

    /* segment */
    INT8 segment;
    /* Store register number or immediate */
    INT32 operand;
    /* Store operand size */
    OPERAND_SIZE operand_size;
    /* Store operand type */
    INT8 type;
    /* use for addressing */
    INT8 scale;
    INT8 index;
    INT8 base;
    /* displacement */
    INT32 displacement;
    int displacement_size;
    /* addressing size, if exists */
    OPERAND_SIZE addressing_size;
    /* whether this operand is a default register */
    bool isDefault;
};

#endif
