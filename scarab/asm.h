/* @(#)asm.h
 */

#ifndef _ASM_H
#define _ASM_H 1

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "dis.h"
#include "type.h"

enum RETURN_CODE{
    RC_SUCCESS, RC_LEX_ERR, RC_TOKEN_NOT_RECOGNIZED, RC_MULTIPLE_LABEL, RC_LEX_EMPTY,
    RC_PARSER_ERR, RC_PARSER_EMPTY, RC_PARSER_EXPECTED_WRONG, RC_PARSER_ADDRESSING_ERR,
    RC_PARSER_MULTIPLE_PREFIX, RC_PARSER_UNEXPECTED_END, RC_PARSER_TOO_MANY_OPERAND,
    RC_PARSER_OPERAND_ERR, RC_SEM_EMPTY
};

// letter -> [A-Za-z]
// digit  -> [0-9]
// end    -> ' ' | '\t' | '\n'

enum TOKEN_TYPE{
    TT_UNDEFINED,
    TT_ERR,
    TT_HEX,                // 0[xX][0-9a-f]* | [0-9a-f]*[h]
    TT_LABEL,              // leter(leter|digit)*:
    TT_PREFIX,             // letter(letter | digit)*
    TT_MNEMONIC,           // letter(letter | digit)*
    TT_SIZE,               // letter(letter | digit)*
    TT_SPECIAL,            // letter(letter | digit)*
    TT_SEGMENT,            // letter(letter | digit)*
    TT_GENERAL_REGISTER,   // letter(letter | digit)*
    TT_FLOAT_REGISTER,     // letter(letter | digit)*[\(digit\)|digit]
    TT_SSEMMX_REGISTER,
    TT_CR,
    TT_DR,
    TT_MARK,               // + | - | * | , | [ | ] | : | $
    TT_COMMENT,            // ; *
};

#define TC_UNDEFINED      -1
#define TC_ANY            -2

enum TOKEN_CLASS_SIZE{
    TSZ_BYTE, TSZ_WORD, TSZ_DWORD, TSZ_FWORD, TSZ_QWORD, TSZ_DQWORD,
    TSZ_TBYTE, TSZ_14BYTE, TSZ_28BYTE, TSZ_94BYTE, TSZ_108BYTE, TSZ_512BYTE,
    TSZ_SHORT, TSZ_LONG, TSZ_NEAR, TSZ_FAR, TSZ_OTHER
};

enum TOKEN_CLASS_SPECIAL{
    TSP_PTR
};

enum TOKEN_CLASS_CR{
    TCR_CR0, TCR_CR1, TCR_CR2, TCR_CR3, TCR_CR4, TCR_CR5, TCR_CR6, TCR_CR7
};

enum TOKEN_CLASS_DR{
    TDR_DR0, TDR_DR1, TDR_DR2, TDR_DR3, TDR_DR4, TDR_DR5, TDR_DR6, TDR_DR7
};

enum TOKEN_CLASS_PREFIX{
    TPX_LOCK, TPX_REPNE, TPX_REPNZ, TPX_REPN, TPX_REP, TPX_REPE, TPX_REPZ
};

enum TOKEN_CLASS_HEX{
    THX_DISPLACEMENT, THX_IMMEDIATE
};

enum TOKEN_CLASS_GENERAL_REGISTER{
    TGR_AL, TGR_CL, TGR_DL, TGR_BL, TGR_AH, TGR_CH, TGR_DH, TGR_BH,
    TGR_AX, TGR_CX, TGR_DX, TGR_BX, TGR_SP, TGR_BP, TGR_SI, TGR_DI,
    TGR_EAX, TGR_ECX, TGR_EDX, TGR_EBX, TGR_ESP, TGR_EBP, TGR_ESI, TGR_EDI,
};

enum TOKEN_CLASS_FLOAT_REGISTER{
    TFR_ST0, TFR_ST1, TFR_ST2, TFR_ST3, TFR_ST4, TFR_ST5, TFR_ST6, TFR_ST7
};

enum TOKEN_CLASS_SSEMMX_REGISTER{
    TXR_MM0, TXR_MM1, TXR_MM2, TXR_MM3, TXR_MM4, TXR_MM5, TXR_MM6, TXR_MM7,
    TXR_XMM0, TXR_XMM1, TXR_XMM2, TXR_XMM3, TXR_XMM4, TXR_XMM5, TXR_XMM6, TXR_XMM7
};

enum TOKEN_CLASS_SEGMENT{
    TSG_ES, TSG_CS, TSG_SS, TSG_DS, TSG_FS, TSG_GS, TSG_SEG
};

enum TOKEN_CLASS_MARK{
    TMK_ADD, TMK_SUB, TMK_MUL, TMK_COMMA, TMK_L_BRACKET, TMK_R_BRACKET,
    TMK_COLON, TMK_DOLLAR
};

#define INCLUDE_CURRENT         1
#define NOT_INCLUDE_CURRENT     0

typedef struct TOKEN{
    char  *name_;
    INT8 type_;
    INT32 class_;
    int instr_index_;
    int instrstr_index_;
    int start_point_;
    int end_point_;

    TOKEN(char *name, INT8 type, int instr_index, int instrstr_index, int start_point, int end_point){
	name_ = name;
	type_ = type;
	class_ = TC_UNDEFINED;
	instr_index_ = instr_index;
	instrstr_index_ = instrstr_index;
	start_point_ = start_point;
	end_point_ = end_point;
    }
    TOKEN(char *name, INT32 type, INT8 c, int instr_index, int instrstr_index, int start_point, int end_point){
	name_ = name;
	type_ = type;
	class_ = c;
	instr_index_ = instr_index;
	instrstr_index_ = instrstr_index;
	start_point_ = start_point;
	end_point_ = end_point;
    }
} TOKEN;

typedef struct INSTRSTR{
    int mnemonic_index_;
    int line_number_;
    char instruction_[200];

    INSTRSTR(int line_number, char *instruction){
	line_number_ = line_number;

	memcpy(instruction_, instruction, strlen(instruction));
	instruction_[strlen(instruction)] = '\0';
	lower(instruction_);
    }
} INSTRSTR;

/* -------------- */
typedef struct ASMOPERAND{
    INT8 type_;

    /* register number or immediate */
    INT32 operand_;

    int operand_size_;

    /* for memory */
    INT8 segment_;
    INT8 scale_;
    INT8 index_;
    INT8 base_;
    INT32 displacement_;
    int displacement_size_;
    INT8 addressing_size_;

    bool default_;
    bool signExtended_;
} ASMOPERAND;

typedef struct ASMINSTRUCTION{
    /* prefix */
    INT8 lockAndRepeat_;
    INT8 segmentOverride_;
    INT8 OperandSizeOverride_;
    INT8 AddressSizeOverride_;
    INT8 waitPrefix_;

    /* operands */
    ASMOPERAND *dest_;
    ASMOPERAND *src1_;
    ASMOPERAND *src2_;
    ASMOPERAND *src3_;

    /* instruction type */
    INT8 type_;

    /* has ModR/M ? */
    bool ModRM_;

    /* sign extended ? */
    int s_;

    /* opcode */
    UINT32 opcode_;
    INT8 auxiliaryOpcode_;

    /* mnemonic */
    const char *mnemonic_;

    /* machine code*/
    UINT8* machineCode_;
    int size_;

    /* instrstr index */
    int instrstr_index_;

    /* address */
    UINT32 address_;

    /* optimize */
    INT32 alternativeOpcode_;
} ASMINSTRUCTION;

// use for label
enum LABEL_OR_NOT{
    LBL_PREFIX, LBL_SEGMENT, LBL_COLLIDE, LBL_SUCCESS
};

typedef struct SYMBOL{
    char *name_;
    int token_index_;
    int instr_index_;

    SYMBOL(char *n, int t, int i){
	name_ = n;
	token_index_ = t;
	instr_index_ = i;
    }
} SYMBOL;

typedef struct offsetTable{
    int index_;
    int offset_;

    offsetTable(int i, int o){
	index_ = i;
	    offset_ = o;
    }
} offsetTable;

class Lex
{
 public:
    Lex();
    void pushInstr(char *instruction);
    void tokenize();
    std::vector<TOKEN> *getToken();
    std::vector<SYMBOL> *getSymbol();
    std::vector<INSTRSTR> *getInstr();
    void printToken();
    void printSymbolTable();
    void printInstr();
    void clear();

 private:
    UINT8 scanInstruction();
    char* getTokenName(bool include_current = false);
    void insertToken(INT8 token_type, INT8 token_class, bool include_current = false);
    void insertToken(char *name, INT8 token_type, INT8 token_class, bool include_current = false);
    UINT8 insertSymbol(char *name, int token_index);
    SYMBOL* getSymbolByName(char *name);
    UINT8 updateUndefined();
    bool collideWithReservedWord(char *name, INT8 &return_type, INT8 &return_class);
    //void errorAndExit(int line_number, char *instruction, char *err_msg, int start_point, int end_point, UINT8 err_type);
    void errorAndExit(UINT8 err_type);

    std::vector<TOKEN> token_;
    std::vector<SYMBOL> symtab_;
    std::vector<INSTRSTR> instructions_;
    int start_point_;
    int current_point_;
    int instr_index_;
    int instrstr_index_;
    int current_token_count_;
    char *current_instr_;
};

#define MOVE_TO_NEXT     true
#define NEEDED           true
#define NOT_NEED         false
#define SAVE_OPERAND     true
#define SAVE_PREFIX      true
#define IS_POSITIVE      true

class Parser
{
 public:
    Parser();
    void init(std::vector<TOKEN> *token, std::vector<SYMBOL> *symtab, std::vector<INSTRSTR> *instructions);
    void parse();
    void clear();
    void print();
    std::vector<ASMINSTRUCTION> *getAsmInstr();
 private:
    int getSymbolIndex(char *name);
    bool matchToken(INT8 token_type, INT8 token_class, bool move_to_next = false, bool need = false, bool save = false);
    void parseAssembly();
    void parseInstruction();
    bool parseOperand();
    bool parseMemory();
    bool parseAddressing();
    bool parseNum(UINT32 &data, int &size, bool isPositive = false);
    bool parseSI();
    void errorAndExit(UINT8 err_type, int token_index);
    void printOperand(ASMOPERAND *operand);
    void setAsmOperand(INT8 type, INT32 operand, int size);

    int current_point_;
    int instr_index_;
    ASMINSTRUCTION asminstr_;
    ASMOPERAND *asmoperand_;
    std::vector<ASMINSTRUCTION> asminstrs_;
    std::vector<TOKEN> *token_;
    std::vector<SYMBOL> *symtab_;
    std::vector<INSTRSTR> *instructions_;
};

class Semantic{
 public:
    Semantic();
    void init(std::vector<ASMINSTRUCTION> *asminstrs, std::vector<SYMBOL> *symtab, std::vector<INSTRSTR> *instructions);
    void clear();
    void sematicCheck(UINT32 base = 0x00400000);
    char *outputMachineCode();
 private:
    void matchInstr(ASMINSTRUCTION *asminstr);
    bool matchOperand(ASMINSTRUCTION *inputInstr, ASMINSTRUCTION *compareInstr, ASMOPERAND *inputOperand, ASMOPERAND *compareOperand);
    void errorAndExit(UINT8 err_type, ASMINSTRUCTION *instr);
    ASMINSTRUCTION *copyAsmInstr(ASMINSTRUCTION *instr);
    void deleteAsmInstr(ASMINSTRUCTION *instr);
    void addMachineCode(int size, UINT32 code);
    void encodeInstr(ASMINSTRUCTION *instr);
    void encodeOperand(ASMINSTRUCTION *instr, ASMOPERAND *operand);
    bool instrNotMatch();
    bool operandNotMatch(ASMOPERAND *op1, ASMOPERAND *op2);
    int hexSize(UINT32 opcode);
    void flowInstructionRelativeAddressReencode();
    void balanceOffset(int index, int offset=3);

    UINT32 base_;
    UINT32 address_;
    UINT8 machineCode_[20];
    int machineCodeSize_;
    int instrIndex_;
    UINT8 modrm_;
    bool sib_exist_;
    UINT8 sib_;
    int displacement_;
    int displacement_size_;
    int immediate_size1_;
    int immediate_size2_;
    int immediate1_;
    int immediate2_;
    bool signExtended_;
    bool operand_size_not_specify_;
    std::vector<ASMINSTRUCTION> *asminstrs_;
    std::vector<SYMBOL> *symtab_;
    std::vector<INSTRSTR> *instructions_;
    std::vector<ASMINSTRUCTION*> alternative_;
    std::vector<ASMINSTRUCTION*> flowinstr_;
    std::vector<int> flowinstrIndex_;
    std::vector<UINT32> originalAddress_;
};

bool strcmpMnemonic(const char *test, const char *compare);
void clearAsmInstr(ASMINSTRUCTION *instr);
void clearAsmOperand(ASMOPERAND *operand);

#endif /* _ASM_H */
