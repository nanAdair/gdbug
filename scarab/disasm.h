#ifndef DISASM_H
#define DISASM_H

#include "type.h"
#include "dis.h"
#include "operand.h"
#include "instruction.h"
#include "utility.h"

class Disasm
{
public:
    Disasm();
    void testDisasm();
    INT32 disassembler(INT8 MACHINECODE[], int CODESIZE, INT32 addr, INT32 baseAddress, INSTRUCTION *instr);
    void disassembleMachineCode(char *m, char *b, bool print = true);
    void disassemblePlainFile(char *p, char *b);
private:
    int currentByte, codeSize;
    INSTRUCTION *instruction;
    INT8 *machineCode;
    INT8 waitPrefix;
    INT8 lockAndRepeat;
    INT8 segmentOverride;
    INT8 OperandSizeOverride;
    INT8 AddressSizeOverride;
    INT32 address, base;
    char *assemblyCode, *ret_machineCode;
    int hasPrefix;

    INT8 decodePrefix();
    void copyInstruction(INSTRUCTION *instr);
    INT8 decodeModRM(Operand *operand, INT8 operand_number);
    bool needToChangeSign(Operand *operand, int size, INT8 type);
    void preprocessingFloatPointInstruction();
    void addRegister(Operand *operand);
    INT8 specialMnemonicProcess();
    INT8 SSEAndMMXInstructionProcess();
    void allocateOperand(Operand *operand);
    void copyAssembleAndMachineCode(bool onlyPrefix);
    void copyPrefix(bool onlyPrefix);
    void copyOperand(Operand *operand, INT8 operand_number);
    INT8 getImmediateOrDisplacement(Operand *operand, INT8 type, INT8 size);
    void addImmediate(char *str,Operand *operand, INT8 size, bool noRegister, INT8 type, bool fixedSize, bool isAbsolute);
    void addRetCode();
    INT8 validatePrefix();
    void setHandlerIndex();
};

#endif // DISASM_H
