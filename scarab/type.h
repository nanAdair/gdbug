/*
 * =====================================================================================
 *
 *       Filename:  type.h
 *
 *    Description:  types that used most
 *
 *        Version:  1.0
 *        Created:  03/02/2014 11:39:42 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn
 *   Organization:
 *
 * =====================================================================================
*/

#ifndef GDBUG_SCARAB_TYPE_H
#define GDBUG_SCARAB_TYPE_H

#include <list>
#include <memory>
using std::shared_ptr;
//using namespace std;


// Normalize the data type
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned int    UINT32;
typedef char            INT8;
typedef short           INT16;
typedef int             INT32;
typedef long long       INT64;

typedef UINT32			ETYPE;
typedef UINT16			EFLAG;
typedef	UINT16			IFLAG;
typedef INT64			BFLAG;
typedef UINT16			BTYPE;

// ========== SCInstr & Operand ==========
#define InstrIterT std::list< shared_ptr<INSTRUCTION> >::iterator
#define InstrRIterT std::list< shared_ptr<INSTRUNCTION> >::reverse_iterator
#define InstrListT std::list< shared_ptr<SCInstr> >
#define INSTRLIST (InstrList::sharedInstrList())

#define INSTR_FUNCTION(ins) ((ins)->get_block()->get_function())


/*
 *  Instruction Flags
 */
/* If the instruction is indirect, e.g jmp *0xf00 */
#define INDIRECT                      (1LL << 0)
/*
 *  If the instruction begins a basic block.  Note that some basic block
 *  should have its firstInstr field pointing to this instruction
 */
#define BBL_START                     (1LL << 1)
/*
 *  If the instruction ends a basic block. Note that some basic block
 *  should have its lastInstr field pointing to this instruction
 */
#define BBL_END                       (1LL << 2)
/* If this instruction starts a function */
#define FUN_START                     (1LL << 3)
/* If this is the last instruction in a function */
#define FUN_END                       (1LL << 4)



#define MAX_INSTRUCTION_SIZE sizeof(INSTRUCTION)

enum {
    HANDLER_8,
    HANDLER_16,
    HANDLER_32
};

/* prefix */
#define PREFIX_WAIT                 0x9b
#define PREFIX_LOCK                 0xf0
#define PREFIX_REPN                 0xf2
#define PREFIX_REP                  0xf3
#define PREFIX_CS                   0x2e
#define PREFIX_SS                   0x36
#define PREFIX_DS                   0x3e
#define PREFIX_ES                   0x26
#define PREFIX_FS                   0x64
#define PREFIX_GS                   0x65
#define PREFIX_OPERAND              0x66
#define PREFIX_ADDRESS              0x67
#define LOCK                        0
#define REP                         1
#define REPE                        2
#define REPNE                       3
#define ES                          0
#define CS                          1
#define SS                          2
#define DS                          3
#define FS                          4
#define GS                          5
#define NOT_ONLY_PREFIX             0
#define ONLY_PREFIX                 1

/* register */
#define ADDRESSING_AX               9
#define ADDRESSING_CX               10
#define ADDRESSING_DX               11
#define ADDRESSING_BX               7
#define ADDRESSING_SP               12
#define ADDRESSING_BP               6
#define ADDRESSING_SI               4
#define ADDRESSING_DI               5
// Control Register
#define CR0                         0
#define CR1                         1
#define CR2                         2
#define CR3                         3
// for xlat
#define EBX_PLUS_AL                 8
// for pushad/pushfd, popad/popfd
#define ALL_32BITS_REGISTER         9
#define ALL_EFLAGS                  10

enum REGISTER_FOR_INT8{
    AL, CL, DL, BL, AH, CH, DH, BH, ALL_8BITS_REGISTER
};
enum REGISTER_FOR_INT16{
    AX, CX, DX, BX, SP, BP, SI, DI, ALL_16BITS_REGISTER
};
enum REGISTER_FOR_INT32{
    EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
};
enum REGISTER_FOR_FLOAT{
    ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7
};
enum REGISTER_FOR_SSE{
    XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7
};

/* operand type */
enum OPERAND_TYPE{
    OPERAND_IMMEDIATE, OPERAND_MEMORY, OPERAND_REGISTER, OPERAND_SEGMENT,
    OPERAND_FLOAT, OPERAND_FLOW, OPERAND_MEMORY_OFFSET, OPERAND_LDTR,
    OPERAND_TR, OPERAND_GDTR, OPERAND_IDTR, OPERAND_MSW,
    OPERAND_CR, OPERAND_DR, OPERAND_SSE_MEMORY, OPERAND_SSE_REGISTER, OPERAND_MSR,
    OPERAND_PMC, OPERAND_MMX_MEMORY, OPERAND_MMX_REGISTER, OPERAND_UNDEFINED,
    OPERAND_LABEL, OPERAND_MEMORY_REGISTER, OPERAND_MEMORY_MEMORY, OPERAND_SSEMEM,
    OPERAND_MEMORY_SSE, OPERAND_MMXMEM, OPERAND_MEMORY_MMX, OPERAND_MEMORY_FLOAT,
    OPERAND_FLOATMEM, OPERAND_FLOWIMM, OPERAND_NOT_EXIST
};

/* instruction type */
enum INSTRUCTION_TYPE{
    UNDEFINED_INSTRUCTION,             // the opcode can't be parsed
    NORMAL_INSTRUCTION,                // arithmetic, move, etc
    STACK_OPERATE_INSTRUCTION,         // push, pop, etc
    FLOW_INSTRUCTION,                  // jump, call, return, etc
    IRREPLACEABLE_INSTRUCTION,         // int3, sysenter, in, out, etc
    FLOAT_INSTRUCTION,                 // kind of weird to put it here
    PREFIX_INSTRUCTION,
    SSE1_INSTRUCTION,
    SSE2_INSTRUCTION,
    SSE3_INSTRUCTION,
    SSSE3_INSTRUCTION,
    SSE41_INSTRUCTION,
    SSE42_INSTRUCTION,
    MMX_INSTRUCTION,
    VMX_INSTRUCTION,
    SMX_INSTRUCTION,
    NEED_FURTHER_PARSED,               // this instruction need to be further parsed
    SPECIAL_MNEMONIC                   // special mnemonic
};
/* instruction class */
#define UNDEFINED_CLASS                0xff
enum PREFIX_INSTRUCTION_CLASS{
    CLASS_ES, CLASS_CS, CLASS_SS, CLASS_DS,
    CLASS_FS, CLASS_GS, CLASS_OPERAND_SIZE,
    CLASS_ADDRESS_SIZE, CLASS_LOCK,
    CLASS_REPN, CLASS_REP
};
enum NORMAL_INSTRUCTION_CLASS{
    CLASS_ADD, CLASS_OR, CLASS_ADC, CLASS_SBB,
    CLASS_AND, CLASS_DAA, CLASS_SUB, CLASS_DAS,
    CLASS_XOR, CLASS_AAA, CLASS_CMP, CLASS_AAS,
    CLASS_INC, CLASS_DEC, CLASS_BOUND, CLASS_ARPL,
    CLASS_IMUL, CLASS_TEST, CLASS_XCHG, CLASS_MOV,
    CLASS_LEA, CLASS_CWD, CLASS_CDQ, CLASS_SAHF,
    CLASS_LAHF, CLASS_MOVS, CLASS_CMPS, CLASS_STOS,
    CLASS_LODS, CLASS_SCAS, CLASS_ROL, CLASS_ROR,
    CLASS_RCL, CLASS_RCR, CLASS_SHL, CLASS_SHR,
    CLASS_SAL, CLASS_SAR, CLASS_LES, CLASS_LDS,
    CLASS_AAM, CLASS_AAD, CLASS_SALC, CLASS_XLAT,
    CLASS_NOT, CLASS_NEG, CLASS_MUL, CLASS_DIV,
    CLASS_IDIV, CLASS_LAR, CLASS_LSL, CLASS_ENTER,
    CLASS_LEAVE, CLASS_XGETBV, CLASS_XSETBV,
    CLASS_RDTSCP, CLASS_MOVBE, CLASS_CMPXCHG,
    CLASS_CMPXCHG8B, CLASS_XADD, CLASS_BTC,
    CLASS_BTR, CLASS_BTS, CLASS_SMSW
};
enum STACK_OPERATE_INSTRUCTION_CLASS{
    CLASS_PUSH, CLASS_POP, CLASS_PUSHAD, CLASS_POPAD,
    CLASS_PUSHFD, CLASS_POPFD, CLASS_INFO, CLASS_IRETD
};
enum FLOW_INSTRUCTION_CLASS{
    CLASS_JO, CLASS_JNO, CLASS_JB, CLASS_JNB,
    CLASS_JE, CLASS_JNE, CLASS_JBE, CLASS_JA,
    CLASS_JS, CLASS_JNS, CLASS_JPE, CLASS_JPO,
    CLASS_JL, CLASS_JGE, CLASS_JLE, CLASS_JG,
    CLASS_CALLF, CLASS_RETN, CLASS_RETF, CLASS_LOOPDNE,
    CLASS_LOOPDE, CLASS_LOOPD, CLASS_JEXCZ, CLASS_CALL,
    CLASS_JMP, CLASS_JMPF, CLASS_JMP_SHORT
};
enum FLOAT_INSTRUCTION_CLASS{
    CLASS_FADD, CLASS_FMUL, CLASS_FCOM, CLASS_FCOMP,
    CLASS_FSUB, CLASS_FSUBR, CLASS_FDIV, CLASS_FDIVR,
    CLASS_FLD, CLASS_FXCH, CLASS_FNOP, CLASS_FCMOVB,
    CLASS_FCMOVE, CLASS_FCMOVBE, CLASS_FCMOVU, CLASS_FISUB,
    CLASS_FUCOMPP, CLASS_FIDIV, CLASS_FIFIVR, CLASS_FCMOVNB,
    CLASS_FCMOVNE, CLASS_FCMOVNBE, CLASS_FCMOVNU, CLASS_FUCOMI,
    CLASS_FCOMI, CLASS_FSTP, CLASS_FFREE,
    CLASS_FST, CLASS_FUCOM, CLASS_FUCOMP, CLASS_FNSAVE,
    CLASS_FSAVE, CLASS_FNSTSW, CLASS_FSTSW, CLASS_FADDP, CLASS_FMULP,
    CLASS_FICOM, CLASS_FCOMPP, CLASS_FSUBRP,
    CLASS_FSUBP, CLASS_FDIVRP, CLASS_FDIVP, CLASS_FFREEP,
    CLASS_FIST, CLASS_FISTP, CLASS_FUCOMIP, CLASS_FCOMIP,
    CLASS_FCHS, CLASS_FABS, CLASS_FSTS, CLASS_FXAM,
    CLASS_FLD1, CLASS_FLDL2T, CLASS_FLDL2E, CLASS_FLDPI,
    CLASS_FLDLG2, CLASS_FLDLN2, CLASS_FLDZ, CLASS_F2XM1,
    CLASS_FYL2X, CLASS_FPTAN, CLASS_FPATAN, CLASS_FXTRACT,
    CLASS_FPREM1, CLASS_FDECSTP, CLASS_FINCSTP, CLASS_FPREM,
    CLASS_FYL2XP1, CLASS_FSQRT, CLASS_FSINCOS, CLASS_FRNDINT,
    CLASS_FSCALE, CLASS_FSIN, CLASS_FCOS, CLASS_FNENI,
    CLASS_FNDISI, CLASS_FNCLEX, CLASS_FCLEX, CLASS_FNINIT, CLASS_FINIT, CLASS_FNSETPM,
    CLASS_FRSTOR, CLASS_FIADD, CLASS_FIMUL,CLASS_FICOMP,
    CLASS_FISUBR, CLASS_FIDIVR, CLASS_FLDENV, CLASS_FLDCW,
    CLASS_FNSTENV, CLASS_FSTENV, CLASS_FNSTCW, CLASS_FSTCW, CLASS_FILD, CLASS_FBLD,
    CLASS_FBSTP, CLASS_FTST, CLASS_FAPTAN, CLASS_FENI,
    CLASS_FDISI,
};
enum VMX_INSTRUCTION_CLASS{
    CLASS_VMCALL, CLASS_VMLAUNCH, CLASS_VMRESUME, CLASS_VMXOFF,
    CLASS_INVEPT, CLASS_INVVPID, CLASS_VMREAD, CLASS_VMWRITE,
    CLASS_VMPTRLD, CLASS_CLEAR, CLASS_VMXON, CLASS_VMPTRST
};
enum SSE1_INSTRUCTION_CLASS{
    CLASS_MOVUPS, CLASS_MOVSS, CLASS_MOVHLPS, CLASS_MOVLPS,
    CLASS_UNPCKLPS, CLASS_UNPCKHPS, CLASS_MOVLHPS, CLASS_MOVHPS,
    CLASS_PREFETCHNTA, CLASS_PREFETCHT0, CLASS_PREFETCHT1, CLASS_PREFETCHT2,
    CLASS_MOVAPS, CLASS_CVTPI2PS, CLASS_CVTSI2SS, MOVNTPS,
    CLASS_CVTTPS2PI, CLASS_CVTTSS2SI, CLASS_CVTPS2PI, CLASS_CVTSS2SI,
    CLASS_UCOMISS, CLASS_COMISS, CLASS_MOVMSKPS, CLASS_SQRTPS,
    CLASS_SQRTSS, CLASS_RSQRTPS, CLASS_RSQRTSS, CLASS_RCPPS,
    CLASS_RCPSS, CLASS_ANDPS, CLASS_ANDNPS, CLASS_ORPS,
    CLASS_XORPS, CLASS_ADDPS, CLASS_ADDSS, CLASS_MULPS,
    CLASS_MULSS, CLASS_SUBPS, CLASS_SUBSS, CLASS_MINPS,
    CLASS_MINSS, CLASS_DIVPS, CLASS_DIVSS, CLASS_MAXPS,
    CLASS_MAXSS, CLASS_PSHUFW, CLASS_LDMXCSR, CLASS_STMXCSR,
    CLASS_SFENCE, CLASS_CMPPS, CLASS_CMPSS, CLASS_PINSRW,
    CLASS_SHUFPS, CLASS_PMOVMSKB, CLASS_PMINUB,
    CLASS_PMAXUB, CLASS_PAVGB, CLASS_PAVGW, CLASS_PMULHUW,
    CLASS_MOVNTQ, CLASS_PMINSW, CLASS_PMAXSW, CLASS_PSADBW,
    CLASS_MASKMOVQ, CLASS_UNPCKLPD, CLASS_MOVNTPS
};
enum SSE2_INSTRUCTION_CLASS{
    CLASS_MOVMSKPD, CLASS_SQRTPD, CLASS_SQRTSD, CLASS_ANDPD,
    CLASS_ANDNPD, CLASS_ORPD, CLASS_XORPD, CLASS_ADDPD, CLASS_ADDSD,
    CLASS_MULPD, CLASS_MULSD, CLASS_CVTPS2PD, CLASS_CVTPD2PS,
    CLASS_CVTSS2SD, CLASS_CVTSD2SS, CLASS_CVTDQ2PS, CLASS_CVTPS2DQ,
    CLASS_CVTTPS2DQ, CLASS_SUBPD, CLASS_SUBSD, CLASS_MINPD, CLASS_MINSD,
    CLASS_DIVPD, CLASS_DIVSD, CLASS_MAXPD, CLASS_MAXSD, CLASS_PUNPCKLQDQ,
    CLASS_PUNPCKHQDQ, CLASS_MOVDQA, CLASS_MOVDQU, CLASS_PSHUFLW,
    CLASS_PSHUFHW, CLASS_PSHUFD, CLASS_PSRLDQ, CLASS_PSRLLDQ,
    CLASS_LFENCE, CLASS_MFENCE, CLASS_CLFUSH, CLASS_CMPPD, CLASS_CMPSD,
    CLASS_SHUFPD, CLASS_PADDQ, CLASS_MOVQ2DQ, CLASS_CVTPD2DQ, CLASS_CVTTPD2DQ,
    CLASS_CVTDQ2PD, CLASS_MOVNTDQ, CLASS_PMULUDQ, CLASS_MASKMOVDQU,
    CLASS_PSUBQ, CLASS_PAUSE, CLASS_MOVUPD, CLASS_MOVSD, CLASS_MOVLPD,
    CLASS_MOVHPD, CLASS_UNPCKHPD, CLASS_MOVAPD, CLASS_CVTPI2PD, CLASS_CVTSI2SD,
    CLASS_MOVNTPD, CLASS_CVTTPD2PI, CLASS_CVTTSD2SI, CLASS_CVTPD2PI, CLASS_CVTSD2SI,
    CLASS_UCOMISD, CLASS_COMISD
};
enum SSE3_INSTRUCTION_CLASS{
    CLASS_FISTTP, CLASS_MONITOR, CLASS_MWAIT, CLASS_MOVDDUP, CLASS_MOVSLDUP,
    CLASS_MOVSHDUP, CLASS_HADDPD, CLASS_HADDPS, CLASS_HSUBPD, CLASS_HSUBPS,
    CLASS_ADDSUBPD, CLASS_ADDSUBPS, CLASS_LDDQU,
};
enum SSSE3_INSTRUCTION_CLASS{
    CLASS_PSHUFB, CLASS_PHADDW, CLASS_PHADDD, CLASS_PHADDSW,
    CLASS_PMADDUBSW, CLASS_PHSUBW, CLASS_PHSUBD, CLASS_PHSUBSW,
    CLASS_PSIGNB, CLASS_PSIGNW, CLASS_PSIGND, CLASS_PMULHRSW,
    CLASS_PABSB, CLASS_PABSW, CLASS_PABSD, CLASS_PALIGNR
};
enum SSE41_INSTRUCTION_CLASS{
    CLASS_PBLENDVB, CLASS_BLENDVPS, CLASS_BLENDVPD, CLASS_PTEST,
    CLASS_PMOVSXBW, CLASS_PMOVSXBD, CLASS_PMOVSXBQ, CLASS_PMOVSXWD,
    CLASS_PMOVSXWQ, CLASS_PMOVSXDQ, CLASS_PMULDQ, CLASS_PCMPEQQ,
    CLASS_MOVNTDQA, CLASS_PACKUSDW, CLASS_PMOVZXBW, CLASS_PMOVZXBD,
    CLASS_PMOVZXBQ, CLASS_PMOVZXWD, CLASS_PMOVZXWQ, CLASS_PMOVZXDQ,
    CLASS_PMINSB, CLASS_PMINSD, CLASS_PMINUW,
    CLASS_PMINUD, CLASS_PMAXSB, CLASS_PMAXSD, CLASS_PMAXUW,
    CLASS_PMAXUD, CLASS_PMULLD, CLASS_PHMINPOSUW, CLASS_ROUNDPS,
    CLASS_ROUNDPD, CLASS_ROUNDSS, CLASS_ROUNDSD, CLASS_BLENDPS,
    CLASS_BLENDPD, CLASS_PBLENDW, CLASS_PEXTRB, CLASS_PEXTRD,
    CLASS_PEXTRQ, CLASS_EXTRACTPS, CLASS_PINSRB, CLASS_INSERTPS,
    CLASS_PINSRD, CLASS_PINSRQ, CLASS_DPPS, CLASS_DPPD, CLASS_MPSADBW
};
enum SSE42_INSTRUCTION_CLASS{
    CLASS_PCMPGTQ, CLASS_CRC32, CLASS_PCMPESTRM, CLASS_PCMPESTRI,
    CLASS_PCMPISTRM, CLASS_PCMPISTRI
};
enum MMX_INSTRUCTION_CLASS{
    CLASS_EMMS
};
enum SMX_INSTRUCTION_CLASS{
    CLASS_GETSEC
};
enum CONFLICT_CLASS{
    CLASS_PUNPCKLBW, CLASS_PUNPCKLWD, CLASS_PUNPCKLDQ, CLASS_PACKSSWB,
    CLASS_PCMPGTB, CLASS_PCMPGTW, CLASS_PCMPGTD, CLASS_PACKUSWB,
    CLASS_PUNPCKHBW, CLASS_PUNPCKHWD, CLASS_PUNPCKHDQ, CLASS_PACKSSDW,
    CLASS_MOVD, CLASS_PSRLW, CLASS_PSRAW, CLASS_PSLLW, CLASS_PSRLD,
    CLASS_PSRAD, CLASS_PSLLD, CLASS_PSRLQ, CLASS_PSLLQ, CLASS_PCMPEQB,
    CLASS_PCMPEQW, CLASS_PCMPEQD, CLASS_MOVQ, CLASS_PMULLW, CLASS_PSUBUSB,
    CLASS_PSUBUSW, CLASS_PAND, CLASS_PADDUSB, CLASS_PADDUSW, CLASS_PANDN,
    CLASS_PMULHW, CLASS_PSUBSB, CLASS_PSUBSW, CLASS_POR, CLASS_PADDSB,
    CLASS_PADDSW, CLASS_PXOR, CLASS_PMADDWD, CLASS_PSUBB, CLASS_PSUBW,
    CLASS_PSUBD, CLASS_PADDB, CLASS_PADDW, CLASS_PADDD, CLASS_PEXTRW
};
enum IRREPLACEABLE_INSTRUCTION_CLASS{
    CLASS_NOP, CLASS_INS, CLASS_OUTS, CLASS_WAIT, CLASS_INT3,
    CLASS_INT, CLASS_IN, CLASS_OUT, CLASS_INT1, CLASS_HLT,
    CLASS_CMC, CLASS_CLC, CLASS_STC, CLASS_CLI,
    CLASS_STI, CLASS_CLD, CLASS_STD
};

/* immediate type */
enum IMMEDIATE_TYPE{
    IS_IMMEDIATE, IS_DISPLACEMENT, IS_NEW_CS, IS_NEW_EIP, IS_OFFSET
};
/* operand size */
enum OPERAND_SIZE{
    SIZE_BYTE, SIZE_WORD, SIZE_DWORD, SIZE_FIXED_DWORD,
    SIZE_FWORD, SIZE_FIXED_FWORD, SIZE_QWORD, SIZE_FIXED_QWORD,
    SIZE_TBYTE, SIZE_DQWORD, SIZE_14BYTE, SIZE_28BYTE, SIZE_94BYTE,
    SIZE_108BYTE, SIZE_512BYTE,
    RELATIVE_ADDRESS_SHORT, ABSOLUTE_ADDRESS_FAR, RELATIVE_ADDRESS_NEAR,
    ABSOLUTE_ADDRESS_FAR_DWORD, ABSOLUTE_ADDRESS_FAR_WORD,
    RELATIVE_ADDRESS_NEAR_DWORD, RELATIVE_ADDRESS_NEAR_WORD, SIZE_XMM, SIZE_MM,
    SIZE_TO_BE_DECODE, SIZE_UNDEFINED
};

/* pwd */
#define CF                          0x1
#define PF                          0x2
#define AF                          0x4
#define ZF                          0x8
#define SF                          0x10
#define TF                          0x20
#define IF                          0x40
#define DF                          0x80
#define OF                          0x100
// FPU flags
#define C0                          0x200
#define C1                          0x400
#define C2                          0x800
#define C3                          0x1000
// Control Register flags
#define PE                          0x2000
#define MP                          0x4000
#define EM                          0x8000
#define TS                          0x10000
#define ET                          0x20000
#define NE                          0x40000
#define WP                          0x80000
#define AM                          0x100000
#define NW                          0x200000
#define CD                          0x400000
#define PG                          0x800000

/* return condition */
/* for prefix */
#define NOT_PREFIX                  0
#define COLLIDED_PREFIX             -1
#define CONTINUE_DECODING_PREFIX    1
#define PREFIX_NOT_QUALIFY          0
#define PREFIX_QUALIFY              1
/* for addImmediate */
#define NOT_ENOUGH_CODE             0
#define ADD_SUCCESS                 1
/* for decodeModRM */
//#define OPERAND_NOT_EXIST           1
#define DECODE_SUCCESS              2
#define DEFAULT_REGISTER            3
/* operand order */
#define FIRST_OPERAND               0
#define SECOND_OPERAND              1
#define THIRD_OPERAND               2
#define FOURTH_OPERAND              3
// addImmediate argument
#define HAS_NO_REGISTER             1
#define HAS_REGISTER                0
#define IS_NOT_FIXED_SIZE           0
#define IS_FIXED_SIZE               1
#define IS_NOT_ABSOLUTE             0
#define IS_ABSOLUTE                 1
// Section type
#define SECTION_OTHER               0
#define SECTION_INIT                1
#define SECTION_TEXT                2
#define SECTION_FINI                3
#define SECTION_PLT                 4


// ========== SCBlock ==========

#define BlockListT std::list< shared_ptr<Block> >
#define BlockIterT std::list< shared_ptr<Block> >::iterator
#define BLOCKLIST (BlockList::sharedBlockList())

/*
 *    Basic Block Types
 */
#define BT_INVALID 0 // BBL of this type has been set up incorrectly
#define BT_UNCOND 1  // BBL ending with an unconditional jump
#define BT_NORMAL 2  // BBL with no control transfer at the end
#define BT_BRANCH 3  // BBL ending with a conditional jump instruction
#define BT_RETURN 4  // BBL ending with a return instruction
#define BT_SWITCH 5  // BBL which ends with a jump through a jump table
#define BT_HELL 6    // HELL
#define BT_ENTRY 7   // abstract pseudo entry block for a function
#define BT_EXIT 8    // abstract pseudo exit block for a function
#define BT_CALL 9   // BBL ending with a function call instruction
#define BT_HALT 10  // BBL in which the program terminates
#define BT_SYSCALL 11// basic block that ends in a system call
#define BT_DATABLOCK 12// basic block that ends in a system call


// ========== SCFunction ==========

#define FunListT std::list< shared_ptr<Function> >
#define FunIterT std::list< shared_ptr<Function> >::iterator
#define FUNLIST (FunctionList::sharedFunctionList())


// ========== SCEdge ==========
#define EdgeListT std::list< shared_ptr<Edge> >
#define EdgeIterT std::list< shared_ptr<Edge> >::iterator
#define EDGELIST (EdgeList::sharedEdgeList())

#define ET_INVALID           0
#define ET_FUNCALL           1
#define ET_UNCOND            2
#define ET_NORMAL            3
#define ET_RETURN            4
#define ET_FUNLINK           5
#define ET_DATALINK          6
#define ET_HALTLINK          7
#define ET_TRUE              8
#define ET_FALSE             9
#define ET_EXIT              10
#define ET_FUNDIRECT         11
// **?? COMPENSATE ??**什么作用
#define ET_COMPENSATE        12
#define ET_SWITCH            13
#define ET_HELL              14
#define ET_HELLMAYBE         15
#define ET_ENTRY             16
#define ET_CHAIN             17
#define ET_SYSCALL           18
#define ET_SYSCALL_LINK      19

#endif // !defined(GDBUG_SCARAB_TYPE_H)
