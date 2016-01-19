/*
  -----------------------------------------------------
		   asm-table.cpp

	   Created on: 2014/05/08 02:12:46
		 Author: Zhibin Zhang
	     Email: zzbthechaos@gmail.com

  -----------------------------------------------------
*/

#include "asm-table.h"

const char *g_prefix_lr[PREFIX_NUM] = {"lock", "repne", "repnz", "repn", "rep", "repe", "repz"};
const char *g_general_register[GENERAL_REGISTER_NUM] ={
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di",
    "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};
const char *g_float_register[2][FLOAT_REGISTER_NUM] = {
    {"st0", "st1", "st2", "st3", "st4", "st5", "st6", "st7"},
    {"st(0)", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)"}
};
const char *g_ssemmx_register[SSEMMX_REGISTER_NUM] = {
    "mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7",
    "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"
};
const char *g_size[SIZE_NUM] = {
    "byte", "word", "dword", "fword", "qword", "dqword", "tbyte", "<14-byte>", "<28-byte>",
    "<94-byte>", "<108-byte>", "<512-byte>", "short", "long", "near", "far"
};
const char *g_segment[SEGMENT_NUM] = {
    "es", "cs", "ss", "ds", "fs", "gs", "seg?"
};
const char *g_special[SPECIAL_NUM] = {
    "ptr"
};
const char *g_cr[CR_NUM] = {
    "cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"
};
const char *g_dr[DR_NUM] = {
    "dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7"
};

// general_register
ASMOPERAND general_register_byte_al = {OPERAND_REGISTER, AL, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_cl = {OPERAND_REGISTER, CL, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_cl_default = {OPERAND_REGISTER, CL, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, true, false};
ASMOPERAND general_register_byte_dl = {OPERAND_REGISTER, DL, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_bl = {OPERAND_REGISTER, BL, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_ah = {OPERAND_REGISTER, AH, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_ch = {OPERAND_REGISTER, CH, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_dh = {OPERAND_REGISTER, DH, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte_bh = {OPERAND_REGISTER, BH, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_byte = {OPERAND_REGISTER, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_word = {OPERAND_REGISTER, -1, SIZE_WORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_word_ax = {OPERAND_REGISTER, AX, SIZE_WORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_word_dx = {OPERAND_REGISTER, DX, SIZE_WORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_eax = {OPERAND_REGISTER, EAX, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_ecx = {OPERAND_REGISTER, ECX, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_edx = {OPERAND_REGISTER, EDX, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_ebx = {OPERAND_REGISTER, EBX, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_esp = {OPERAND_REGISTER, ESP, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_ebp = {OPERAND_REGISTER, EBP, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_esi = {OPERAND_REGISTER, ESI, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword_edi = {OPERAND_REGISTER, EDI, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_dword = {OPERAND_REGISTER, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_fixed_dword = {OPERAND_REGISTER, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND general_register_word_dx_default = {OPERAND_REGISTER, DX, SIZE_WORD, -1, -1, -1, -1, -1, -1, true, false};

// memory
ASMOPERAND memory_byte = {OPERAND_MEMORY, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_word = {OPERAND_MEMORY, -1, SIZE_WORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_28 = {OPERAND_MEMORY, -1, SIZE_28BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_dword = {OPERAND_MEMORY, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_fixed_dword = {OPERAND_MEMORY, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_fword = {OPERAND_MEMORY, -1, SIZE_FWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_fixed_fword = {OPERAND_MEMORY, -1, SIZE_FIXED_FWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_qword = {OPERAND_MEMORY, -1, SIZE_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_fixed_qword = {OPERAND_MEMORY, -1, SIZE_FIXED_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_dqword = {OPERAND_MEMORY, -1, SIZE_DQWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_byte_ds_esi_default = {OPERAND_MEMORY, -1, SIZE_BYTE, DS, -1, -1, ESI, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memory_word_ds_esi_default = {OPERAND_MEMORY, -1, SIZE_WORD, DS, -1, -1, ESI, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memory_dword_ds_esi_default = {OPERAND_MEMORY, -1, SIZE_DWORD, DS, -1, -1, ESI, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memory_byte_es_edi_default = {OPERAND_MEMORY, -1, SIZE_BYTE, ES, -1, -1, EDI, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memory_word_es_edi_default = {OPERAND_MEMORY, -1, SIZE_WORD, ES, -1, -1, EDI, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memory_dword_es_edi_default = {OPERAND_MEMORY, -1, SIZE_DWORD, ES, -1, -1, EDI, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memory_register_dword = {OPERAND_MEMORY_REGISTER, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND memory_register_fixed_dword = {OPERAND_MEMORY_REGISTER, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND memory_memory_fixed_dword = {OPERAND_MEMORY_MEMORY, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_byte = {OPERAND_MEMORY_MEMORY, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_word = {OPERAND_MEMORY_MEMORY, -1, SIZE_WORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_28 = {OPERAND_MEMORY_MEMORY, -1, SIZE_28BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_dword = {OPERAND_MEMORY_MEMORY, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_qword = {OPERAND_MEMORY_MEMORY, -1, SIZE_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_fixed_fword = {OPERAND_MEMORY_MEMORY, -1, SIZE_FIXED_FWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_fixed_qword = {OPERAND_MEMORY_MEMORY, -1, SIZE_FIXED_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_108 = {OPERAND_MEMORY_MEMORY, -1, SIZE_108BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_dqword = {OPERAND_MEMORY_MEMORY, -1, SIZE_DQWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_tbyte = {OPERAND_MEMORY_MEMORY, -1, SIZE_TBYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_memory_512 = {OPERAND_MEMORY_MEMORY, -1, SIZE_512BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND memory_byte_ebx_al = {OPERAND_MEMORY, -1, SIZE_BYTE, -1, -1, -1, EBX_PLUS_AL, -1, -1, SIZE_DWORD, true, false};
ASMOPERAND memsse = {OPERAND_MEMORY_SSE, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND memmmx = {OPERAND_MEMORY_MMX, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND memfloat = {OPERAND_MEMORY_FLOAT, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};

// sse
ASMOPERAND sse_register = {OPERAND_SSE_REGISTER, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND ssemem_fixed_dword = {OPERAND_SSEMEM, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND ssemem_fixed_qword = {OPERAND_SSEMEM, -1, SIZE_FIXED_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND ssemem_dqword = {OPERAND_SSEMEM, -1, SIZE_DQWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND ssemem_word = {OPERAND_SSEMEM, -1, SIZE_WORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};

// mmx
ASMOPERAND mmx_register = {OPERAND_MMX_REGISTER, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND mmxmem_fixed_dword = {OPERAND_MMXMEM, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND mmxmem_fixed_qword = {OPERAND_MMXMEM, -1, SIZE_FIXED_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND mmxmem_dqword = {OPERAND_MMXMEM, -1, SIZE_DQWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};

// float point
ASMOPERAND float_register = {OPERAND_FLOAT, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND float_register_st0 = {OPERAND_FLOAT, ST0, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND floatmem_word = {OPERAND_FLOATMEM, -1, SIZE_WORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND floatmem_fixed_dword = {OPERAND_FLOATMEM, -1, SIZE_FIXED_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND floatmem_fixed_qword = {OPERAND_FLOATMEM, -1, SIZE_FIXED_QWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};
ASMOPERAND floatmem_tbyte= {OPERAND_FLOATMEM, -1, SIZE_TBYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false, false};

// immediate
ASMOPERAND immediate_1 = {OPERAND_IMMEDIATE, 1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND immediate_byte = {OPERAND_IMMEDIATE, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND immediate_word = {OPERAND_IMMEDIATE, -1, SIZE_WORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND immediate_dword = {OPERAND_IMMEDIATE, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};

// flow
ASMOPERAND flow_rel_short = {OPERAND_FLOW, -1, RELATIVE_ADDRESS_SHORT, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND flow_rel_near_dword = {OPERAND_FLOW, -1, RELATIVE_ADDRESS_NEAR_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND flow_abs_far_dword = {OPERAND_FLOW, -1, ABSOLUTE_ADDRESS_FAR_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND flow_imm_dword = {OPERAND_FLOWIMM, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND flow_imm_byte = {OPERAND_FLOWIMM, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND flow_label_dword = {OPERAND_LABEL, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND flow_label_byte = {OPERAND_LABEL, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, -1, false, false};

// segment
ASMOPERAND sreg = {OPERAND_SEGMENT, -1, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND sreg_cs = {OPERAND_SEGMENT, CS, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND sreg_ss = {OPERAND_SEGMENT, SS, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND sreg_ds = {OPERAND_SEGMENT, DS, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND sreg_es = {OPERAND_SEGMENT, ES, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND sreg_fs = {OPERAND_SEGMENT, FS, -1, -1, -1, -1, -1, -1, -1, false, false};
ASMOPERAND sreg_gs = {OPERAND_SEGMENT, GS, -1, -1, -1, -1, -1, -1, -1, false, false};

// memory offset
ASMOPERAND memory_offset_byte = {OPERAND_MEMORY_OFFSET, -1, SIZE_BYTE, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false};
ASMOPERAND memory_offset_dword = {OPERAND_MEMORY_OFFSET, -1, SIZE_DWORD, -1, -1, -1, -1, -1, -1, SIZE_DWORD, false};

// control register
ASMOPERAND control_register = {OPERAND_CR, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};

// debug register
ASMOPERAND debug_register = {OPERAND_DR, -1, -1, -1, -1, -1, -1, -1, -1, -1, false, false};

ASMINSTRUCTION g_instr[MNEMONIC_NUM] = {
    // add
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x00, -1, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x01, -1, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x02, -1, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x03, -1, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x04, -1, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x05, -1, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x0, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x0, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x0, "add", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x0, "add", NULL, -1, -1, 0, -1},
    // or
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x08, -1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x09, -1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0a, -1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0b, -1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0c, -1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0d, -1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x1, "or", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x1, "or", NULL, -1, -1, 0, -1},
    // adc
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x10, -1, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x11, -1, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x12, -1, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x13, -1, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x14, -1, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x15, -1, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x2, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x2, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x2, "adc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x2, "adc", NULL, -1, -1, 0, -1},
    // sbb
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x18, -1, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x19, -1, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x1a, -1, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x1b, -1, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x1c, -1, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x1d, -1, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x3, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x3, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x3, "sbb", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x3, "sbb", NULL, -1, -1, 0, -1},
    // and
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x20, -1, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x21, -1, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x22, -1, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x23, -1, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x24, -1, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x25, -1, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x4, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x4, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x4, "and", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x4, "and", NULL, -1, -1, 0, -1},
    // daa
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x27, -1, "daa", NULL, -1, -1, 0, -1},
    // sub
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x28, -1, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x29, -1, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x2a, -1, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x2b, -1, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x2c, -1, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x2d, -1, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x5, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x5, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x5, "sub", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x5, "sub", NULL, -1, -1, 0, -1},
    // das
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x2f, -1, "das", NULL, -1, -1, 0, -1},
    // xor
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x30, -1, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x31, -1, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x32, -1, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x33, -1, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x34, -1, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x35, -1, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x6, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x6, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x6, "xor", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x6, "xor", NULL, -1, -1, 0, -1},
    // aaa
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x37, -1, "aaa", NULL, -1, -1, 0, -1},
    // cmp
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x38, -1, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x39, -1, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x3a, -1, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x3b, -1, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x3c, -1, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x3d, -1, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0x80, 0x7, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x81, 0x7, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x82, 0x7, "cmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, 1, 0x83, 0x7, "cmp", NULL, -1, -1, 0, -1},
    // aas
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x3f, -1, "aas", NULL, -1, -1, 0, -1},
    // inc
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x40, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ecx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x41, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_edx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x42, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ebx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x43, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_esp, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x44, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ebp, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x45, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_esi, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x46, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_edi, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x47, -1, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xfe, 0, "inc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xff, 0, "inc", NULL, -1, -1, 0, -1},
    // dec
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x48, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ecx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x49, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_edx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x4a, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ebx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x4b, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_esp, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x4c, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ebp, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x4d, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_esi, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x4e, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_edi, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x4f, -1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xfe, 1, "dec", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xff, 1, "dec", NULL, -1, -1, 0, -1},
    // push
    {0, 0, 0, 0, 0, &sreg_es, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x06, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_cs, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x0e, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_ss, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x16, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_ds, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x1e, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_fs, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x0fa0, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_gs, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x0fa8, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x50, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ecx, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x51, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edx, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x52, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebx, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x53, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esp, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x54, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebp, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x55, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esi, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x56, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edi, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x57, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &immediate_dword, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x68, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &immediate_byte, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x6a, -1, "push", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, true, -1, 0xff, 6, "push", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x6660, -1, "pushaw,pusha", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x60, -1, "pushad,pusha", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x669c, -1, "pushfw,pushf", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x9c, -1, "pushfd,pushf", NULL, -1, -1, 0, -1},
    // pop
    {0, 0, 0, 0, 0, &sreg_es, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x07, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_ss, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x17, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_ds, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x1f, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_fs, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x0fa1, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg_gs, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x0fa9, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x58, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ecx, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x59, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edx, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x5a, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebx, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x5b, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esp, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x5c, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebp, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x5d, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esi, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x5e, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edi, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x5f, -1, "pop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, true, -1, 0x8f, 0, "pop", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x6661, -1, "popaw,popa", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x61, -1, "popad,popa", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x669d, -1, "popfw,popf", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, STACK_OPERATE_INSTRUCTION, false, -1, 0x9d, -1, "popfd,popf", NULL, -1, -1, 0, -1},
    // bound
    {0, 0, 0, 0, 0, &general_register_dword, &memory_memory_qword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x62, -1, "bound", NULL, -1, -1, 0, -1},
    // arpl
    {0, 0, 0, 0, 0, &memory_word, &general_register_word, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x63, -1, "arpl", NULL, -1, -1, 0, -1},
    // imul
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, &immediate_dword, NULL, NORMAL_INSTRUCTION, true, -1, 0x69, -1, "imul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, &immediate_byte, NULL, NORMAL_INSTRUCTION, true, -1, 0x6b, -1, "imul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 0x5, "imul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 0x5, "imul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL,
     NORMAL_INSTRUCTION, true, -1, 0x0faf, -1, "imul", NULL, -1, -1, 0, -1},
    // test
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x84, -1, "test", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x85, -1, "test", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xa8, -1, "test", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, 0, 0xa9, -1, "test", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 0, "test", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 0, "test", NULL, -1, -1, 0, -1},
    // jmp
    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x70, -1, "jo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f80, -1, "jo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f80, -1, "jo", NULL, -1, -1, 0, 0x70},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f80, -1, "jo", NULL, -1, -1, 0, 0x70},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x71, -1, "jno", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f81, -1, "jno", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f81, -1, "jno", NULL, -1, -1, 0, 0x71},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f81, -1, "jno", NULL, -1, -1, 0, 0x71},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x72, -1, "jb,jnae,jc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f82, -1, "jb,jnae,jc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f82, -1, "jb,jnae,jc", NULL, -1, -1, 0, 0x72},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f82, -1, "jb,jnae,jc", NULL, -1, -1, 0, 0x72},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x73, -1, "jnb,jae,jnc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f83, -1, "jnb,jae,jnc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f83, -1, "jnb,jae,jnc", NULL, -1, -1, 0, 0x73},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f83, -1, "jnb,jae,jnc", NULL, -1, -1, 0, 0x73},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x74, -1, "jz,je", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f84, -1, "jz,je", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f84, -1, "jz,je", NULL, -1, -1, 0, 0x74},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f84, -1, "jz,je", NULL, -1, -1, 0, 0x74},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x75, -1, "jnz,jne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f85, -1, "jnz,jne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f85, -1, "jnz,jne", NULL, -1, -1, 0, 0x75},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f85, -1, "jnz,jne", NULL, -1, -1, 0, 0x75},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x76, -1, "jbe,jna", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f86, -1, "jbe,jna", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f86, -1, "jbe,jna", NULL, -1, -1, 0, 0x76},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f86, -1, "jbe,jna", NULL, -1, -1, 0, 0x76},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x77, -1, "jnbe,ja", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f87, -1, "jnbe,ja", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f87, -1, "jnbe,ja", NULL, -1, -1, 0, 0x77},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f87, -1, "jnbe,ja", NULL, -1, -1, 0, 0x77},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x78, -1, "js", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f88, -1, "js", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f88, -1, "js", NULL, -1, -1, 0, 0x78},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f88, -1, "js", NULL, -1, -1, 0, 0x78},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x79, -1, "jns", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f89, -1, "jns", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f89, -1, "jns", NULL, -1, -1, 0, 0x79},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f89, -1, "jns", NULL, -1, -1, 0, 0x79},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x7a, -1, "jp,jpe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8a, -1, "jp,jpe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8a, -1, "jp,jpe", NULL, -1, -1, 0, 0x7a},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8a, -1, "jp,jpe", NULL, -1, -1, 0, 0x7a},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x7b, -1, "jnp,jpo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8b, -1, "jnp,jpo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8b, -1, "jnp,jpo", NULL, -1, -1, 0, 0x7b},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8b, -1, "jnp,jpo", NULL, -1, -1, 0, 0x7b},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x7c, -1, "jl,jnge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8c, -1, "jl,jnge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8c, -1, "jl,jnge", NULL, -1, -1, 0, 0x7c},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8c, -1, "jl,jnge", NULL, -1, -1, 0, 0x7c},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x7d, -1, "jnl,jge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8d, -1, "jnl,jge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8d, -1, "jnl,jge", NULL, -1, -1, 0, 0x7d},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8d, -1, "jnl,jge", NULL, -1, -1, 0, 0x7d},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x7e, -1, "jle,jng", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8e, -1, "jle,jng", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8e, -1, "jle,jng", NULL, -1, -1, 0, 0x7e},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8e, -1, "jle,jng", NULL, -1, -1, 0, 0x7e},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x7f, -1, "jnle,jg", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8f, -1, "jnle,jg", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8f, -1, "jnle,jg", NULL, -1, -1, 0, 0x7f},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x0f8f, -1, "jnle,jg", NULL, -1, -1, 0, 0x7f},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe3, -1, "jecxz", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe3, -1, "jecxz", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe3, -1, "jecxz", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e3, -1, "jcxz", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e3, -1, "jcxz", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e3, -1, "jcxz", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xeb, -1, "jmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe9, -1, "jmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_abs_far_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xea, -1, "jmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe9, -1, "jmp", NULL, -1, -1, 0, 0xeb},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe9, -1, "jmp", NULL, -1, -1, 0, 0xeb},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, true, -1, 0xff, 4, "jmp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_fword, NULL, NULL, NULL, FLOW_INSTRUCTION, true, -1, 0xff, 5, "jmp", NULL, -1, -1, 0, -1},
    // call
    {0, 0, 0, 0, 0, &flow_abs_far_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x9a, -1, "call", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_rel_near_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe8, -1, "call", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe8, -1, "call", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe8, -1, "call", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, FLOW_INSTRUCTION, true, -1, 0xff, 2, "call", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_fword, NULL, NULL, NULL, FLOW_INSTRUCTION, true, -1, 0xff, 3, "call", NULL, -1, -1, 0, -1},
    // ret
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xc3, -1, "retn,ret", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &immediate_word, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xc2, -1, "retn,ret", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xcb, -1, "retf", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &immediate_word, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xca, -1, "retf", NULL, -1, -1, 0, -1},
    // iret
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x66cf, -1, "iretw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xcf, -1, "iretd,iret", NULL, -1, -1, 0, -1},
    // enter
    {0, 0, 0, 0, 0, &immediate_word, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xc8, -1, "enter", NULL, -1, -1, 0, -1},
    // leave
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xc9, -1, "leave", NULL, -1, -1, 0, -1},
    // loop
    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe0, -1, "loopdne,loopnz,loopne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe0, -1, "loopdne,loopnz,loopne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe0, -1, "loopdne,loopnz,loopne", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe1, -1, "loopz,loope,loopde", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe1, -1, "loopz,loope,loopde", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe1, -1, "loopz,loope,loopde", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe2, -1, "loop,loopd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe2, -1, "loop,loopd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0xe2, -1, "loop,loopd", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e0, -1, "loopwne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e0, -1, "loopwne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e0, -1, "loopwne", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e1, -1, "loopwe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e1, -1, "loopwe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e1, -1, "loopwe", NULL, -1, -1, 0, -1},

    {0, 0, 0, 0, 0, &flow_rel_short, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e2, -1, "loopw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_imm_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e2, -1, "loopw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &flow_label_byte, NULL, NULL, NULL, FLOW_INSTRUCTION, false, -1, 0x67e2, -1, "loopw", NULL, -1, -1, 0, -1},
    // xchg
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x86, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x87, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x86, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x87, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x90, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ecx, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x91, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_edx, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x92, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ebx, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x93, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_esp, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x94, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_ebp, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x95, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_esi, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x96, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_edi, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x97, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_ecx, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x91, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_edx, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x92, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_ebx, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x93, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_esp, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x94, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_ebp, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x95, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_esi, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x96, -1, "xchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &general_register_dword_eax, &general_register_dword_edi, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x97, -1, "xchg", NULL, -1, -1, 0, -1},
    // mov
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x88, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x89, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x8a, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x8b, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_word, &sreg, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x8c, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sreg, &memory_word, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x8e, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_al, &memory_offset_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xa0, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, &memory_offset_dword, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xa1, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_offset_byte, &general_register_byte_al, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xa2, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_offset_dword, &general_register_dword_eax, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xa3, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb0, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_cl, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb1, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_dl, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb2, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_bl, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb3, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_ah, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb4, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_ch, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb5, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_dh, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb6, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_bh, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb7, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb8, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ecx, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xb9, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edx, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xba, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebx, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xbb, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esp, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xbc, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebp, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xbd, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esi, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xbe, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edi, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xbf, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xc6, 0x0, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xc7, 0x0, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &control_register, NULL, NULL,
     NORMAL_INSTRUCTION, true, -1, 0x0f20, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &debug_register, NULL, NULL,
     NORMAL_INSTRUCTION, true, -1, 0x0f21, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &control_register, &memory_register_fixed_dword, NULL, NULL,
     NORMAL_INSTRUCTION, true, -1, 0x0f22, -1, "mov", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &debug_register, &memory_register_fixed_dword, NULL, NULL,
     NORMAL_INSTRUCTION, true, -1, 0x0f23, -1, "mov", NULL, -1, -1, 0, -1},
    // lea
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x8d, -1, "lea", NULL, -1, -1, 0, -1},
    // cbw
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x6698, -1, "cbw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x98, -1, "cwde", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x6699, -1, "cwd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x99, -1, "cdq", NULL, -1, -1, 0, -1},
    // sahf
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x9e, -1, "sahf", NULL, -1, -1, 0, -1},
    // lahf
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x9f, -1, "lahf", NULL, -1, -1, 0, -1},
    // movs
    {REP_PERMIT, 0, 0, 0, 0, &memory_dword_es_edi_default, &memory_dword_ds_esi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xa5, -1, "movs", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_byte_es_edi_default, &memory_byte_ds_esi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xa4, -1, "movs,movsb", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_word_es_edi_default, &memory_word_ds_esi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x66a5, -1, "movs,movsw", NULL, -1, -1, 0, -1},
    // cmps
    {REP_PERMIT | REPN_PERMIT, 0, 0, 0, 0, &memory_dword_ds_esi_default, &memory_dword_es_edi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xa7, -1, "cmps,cmpsd", NULL, -1, -1, 0, -1},
    {REP_PERMIT | REPN_PERMIT, 0, 0, 0, 0, &memory_byte_ds_esi_default, &memory_byte_es_edi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xa6, -1, "cmps,cmpsb", NULL, -1, -1, 0, -1},
    {REP_PERMIT | REPN_PERMIT, 0, 0, 0, 0, &memory_word_ds_esi_default, &memory_word_es_edi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x66a7, -1, "cmps,cmpsw", NULL, -1, -1, 0, -1},
    // stos
    {REP_PERMIT, 0, 0, 0, 0, &memory_dword_es_edi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xab, -1, "stos,stosd", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_byte_es_edi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xaa, -1, "stos,stosb", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_word_es_edi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x66ab, -1, "stos,stosw", NULL, -1, -1, 0, -1},
    // lods
    {REP_PERMIT, 0, 0, 0, 0, &memory_dword_ds_esi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xad, -1, "lods,lodsd", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_byte_ds_esi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xac, -1, "lods,lodsb", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_word_ds_esi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x66ad, -1, "lods,lodsw", NULL, -1, -1, 0, -1},
    // scas
    {REP_PERMIT | REPN_PERMIT, 0, 0, 0, 0, &memory_dword_es_edi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xaf, -1, "scas,scasd", NULL, -1, -1, 0, -1},
    {REP_PERMIT | REPN_PERMIT, 0, 0, 0, 0, &memory_byte_es_edi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0xae, -1, "scas,scasb", NULL, -1, -1, 0, -1},
    {REP_PERMIT | REPN_PERMIT, 0, 0, 0, 0, &memory_word_es_edi_default, NULL, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x66af, -1, "scas,scasw", NULL, -1, -1, 0, -1},
    // rol
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 0, "rol", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 0, "rol", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 0, "rol", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 0, "rol", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 0, "rol", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 0, "rol", NULL, -1, -1, 0, -1},
    // ror
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 1, "ror", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 1, "ror", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 1, "ror", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 1, "ror", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 1, "ror", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 1, "ror", NULL, -1, -1, 0, -1},
    // rcl
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 2, "rcl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 2, "rcl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 2, "rcl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 2, "rcl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 2, "rcl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 2, "rcl", NULL, -1, -1, 0, -1},
    // rcr
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 3, "rcr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 3, "rcr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 3, "rcr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 3, "rcr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 3, "rcr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 3, "rcr", NULL, -1, -1, 0, -1},
    // shl
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 4, "shl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 4, "shl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 4, "shl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 4, "shl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 4, "shl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 4, "shl", NULL, -1, -1, 0, -1},
    // shr
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 5, "shr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 5, "shr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 5, "shr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 5, "shr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 5, "shr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 5, "shr", NULL, -1, -1, 0, -1},
    // sal
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 6, "sal", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 6, "sal", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 6, "sal", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 6, "sal", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 6, "sal", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 6, "sal", NULL, -1, -1, 0, -1},
    // sar
    {0, 0, 0, 0, 0, &memory_byte, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc0, 7, "sar", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xc1, 7, "sar", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd0, 7, "sar", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_1, NULL, NULL, NORMAL_INSTRUCTION, true, 1, 0xd1, 7, "sar", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd2, 7, "sar", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_byte_cl, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xd3, 7, "sar", NULL, -1, -1, 0, -1},
    // les
    {0, 0, 0, 0, 0, &general_register_dword, &memory_fword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xc4, -1, "les", NULL, -1, -1, 0, -1},
    // lds
    {0, 0, 0, 0, 0, &general_register_dword, &memory_fword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xc5, -1, "lds", NULL, -1, -1, 0, -1},
    // aam
    {0, 0, 0, 0, 0, &immediate_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xd4, -1, "aam", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xd40a, -1, "aam", NULL, -1, -1, 0, -1},
    // aad
    {0, 0, 0, 0, 0, &immediate_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xd5, -1, "aad", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xd50a, -1, "aad", NULL, -1, -1, 0, -1},
    // salc
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xd6, -1, "salc", NULL, -1, -1, 0, -1},
    // xlat
    {0, 0, 0, 0, 0, &memory_byte_ebx_al, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xd7, -1, "xlat,xlatb", NULL, -1, -1, 0, -1},
    // not
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 2, "not", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 2, "not", NULL, -1, -1, 0, -1},
    // neg
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 3, "neg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 3, "neg", NULL, -1, -1, 0, -1},
    // mul
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 4, "mul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 4, "mul", NULL, -1, -1, 0, -1},
    // div
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 6, "div", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 6, "div", NULL, -1, -1, 0, -1},
    // idiv
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf6, 7, "idiv", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf7, 7, "idiv", NULL, -1, -1, 0, -1},
    // nop
    {0, 0, 0, 0, 0, &memory_register_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f1f, -1, "nop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x90, -1, "nop", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SSE2_INSTRUCTION, false, -1, 0xf390, -1, "pause", NULL, -1, -1, 0, -1},
    // ins
    {REP_PERMIT, 0, 0, 0, 0, &memory_dword_es_edi_default, &general_register_word_dx_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x6d, -1, "ins,insd", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_byte_es_edi_default, &general_register_word_dx_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x6c, -1, "ins,insb", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &memory_word_es_edi_default, &general_register_word_dx_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x666d, -1, "ins,insw", NULL, -1, -1, 0, -1},
    // outs
    {REP_PERMIT, 0, 0, 0, 0, &general_register_word_dx_default, &memory_dword_ds_esi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x6f, -1, "outs,outsd", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &general_register_word_dx_default, &memory_byte_ds_esi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x6e, -1, "outs,outsb", NULL, -1, -1, 0, -1},
    {REP_PERMIT, 0, 0, 0, 0, &general_register_word_dx_default, &memory_word_ds_esi_default, NULL, NULL,
     NORMAL_INSTRUCTION, false, -1, 0x666f, -1, "outs,outsw", NULL, -1, -1, 0, -1},
    // wait
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x9b, -1, "wait", NULL, -1, -1, 0, -1},
    // int
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xcc, -1, "int3", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &immediate_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xcd, -1, "int", NULL, -1, -1, 0, -1},
    // into
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xce, -1, "into", NULL, -1, -1, 0, -1},
    // in
    {0, 0, 0, 0, 0, &general_register_byte_al, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xe4, -1, "in", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xe5, -1, "in", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_byte_al, &general_register_word_dx, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xec, -1, "in", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_eax, &general_register_word_dx, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xed, -1, "in", NULL, -1, -1, 0, -1},
    // out
    {0, 0, 0, 0, 0, &immediate_byte, &general_register_byte_al, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xe6, -1, "out", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &immediate_byte, &general_register_dword_eax, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xe7, -1, "out", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_word_dx, &general_register_byte_al, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xee, -1, "out", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_word_dx, &general_register_dword_eax, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xef, -1, "out", NULL, -1, -1, 0, -1},
    // int1
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xf1, -1, "int1", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xf1, -1, "icebp", NULL, -1, -1, 0, -1},
    // hlt
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xf4, -1, "hlt", NULL, -1, -1, 0, -1},
    // cmc
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xf5, -1, "cmc", NULL, -1, -1, 0, -1},
    // clc
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xf8, -1, "clc", NULL, -1, -1, 0, -1},
    // stc
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xf9, -1, "stc", NULL, -1, -1, 0, -1},
    // cli
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xfa, -1, "cli", NULL, -1, -1, 0, -1},
    // sti
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xfb, -1, "sti", NULL, -1, -1, 0, -1},
    // cld
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xfc, -1, "cld", NULL, -1, -1, 0, -1},
    // std
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0xfd, -1, "std", NULL, -1, -1, 0, -1},
    // fadd
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 0, "fadd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 0, "fadd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 0, "fadd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 0, "fadd", NULL, -1, -1, 0, -1},
    // fmul
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 1, "fmul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 1, "fmul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 1, "fmul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 1, "fmul", NULL, -1, -1, 0, -1},
    // fcom
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 2, "fcom", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 2, "fcom", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 2, "fcom", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 2, "fcom", NULL, -1, -1, 0, -1},
    // fcomp
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 3, "fcomp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 3, "fcomp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 3, "fcomp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 3, "fcomp", NULL, -1, -1, 0, -1},
    // fsub
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 4, "fsub", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 4, "fsub", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 4, "fsub", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 5, "fsub", NULL, -1, -1, 0, -1},
    // fsubr
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 5, "fsubr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 5, "fsubr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 5, "fsubr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 4, "fsubr", NULL, -1, -1, 0, -1},
    // fdiv
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 6, "fdiv", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 6, "fdiv", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 6, "fdiv", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 7, "fdiv", NULL, -1, -1, 0, -1},
    // fdivr
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd8, 7, "fdivr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd8, 7, "fdivr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 7, "fdivr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdc, 6, "fdivr", NULL, -1, -1, 0, -1},
    // fld
    {0, 0, 0, 0, 0, &floatmem_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 0, "fld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_tbyte, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 5, "fld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 0, "fld", NULL, -1, -1, 0, -1},
    // fxch
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 1, "fxch", NULL, -1, -1, 0, -1},
    // fst
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 2, "fst", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &floatmem_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 2, "fst", NULL, -1, -1, 0, -1},
    // fnop
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9d0, -1, "fnop", NULL, -1, -1, 0, -1},
    // fstp
    {0, 0, 0, 0, 0, &floatmem_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 3, "fstp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_tbyte, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 7, "fstp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &floatmem_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 3, "fstp", NULL, -1, -1, 0, -1},
    // fldenv
    {0, 0, 0, 0, 0, &memory_memory_28, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 4, "fldenv", NULL, -1, -1, 0, -1},
    // fchs
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9e0, -1, "fchs", NULL, -1, -1, 0, -1},
    // fabs
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9e1, -1, "fabs", NULL, -1, -1, 0, -1},
    // ftst
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9e4, -1, "ftst", NULL, -1, -1, 0, -1},
    // fxam
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9e5, -1, "fxam", NULL, -1, -1, 0, -1},
    // fldcw
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 5, "fldcw", NULL, -1, -1, 0, -1},
    // fld1
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9e8, -1, "fld1", NULL, -1, -1, 0, -1},
    // fldl2t
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9e9, -1, "fldl2t", NULL, -1, -1, 0, -1},
    // fldl2e
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9ea, -1, "fldl2e", NULL, -1, -1, 0, -1},
    // fldpi
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9eb, -1, "fldpi", NULL, -1, -1, 0, -1},
    // fldlg2
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9ec, -1, "fldlg2", NULL, -1, -1, 0, -1},
    // fldln2
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9ed, -1, "fldln2", NULL, -1, -1, 0, -1},
    // fldz
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9ee, -1, "fldz", NULL, -1, -1, 0, -1},
    // fnstenv
    {0, 0, 0, 0, 0, &memory_memory_28, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 6, "fnstenv", NULL, -1, -1, 0, -1},
    // fstenv
    {0, 0, 0, 0, 0, &memory_28, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0x9bd9, 6, "fstenv", NULL, -1, -1, 0, -1},
    // f2xm1
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f0, -1, "f2xm1", NULL, -1, -1, 0, -1},
    // fyl2x
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f1, -1, "fyl2x", NULL, -1, -1, 0, -1},
    // fptan
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f2, -1, "fptan", NULL, -1, -1, 0, -1},
    // fpatan
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f3, -1, "fpatan", NULL, -1, -1, 0, -1},
    // fxtract
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f4, -1, "fxtract", NULL, -1, -1, 0, -1},
    // fprem1
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f5, -1, "fprem1", NULL, -1, -1, 0, -1},
    // fdecstp
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f6, -1, "fdecstp", NULL, -1, -1, 0, -1},
    // fincstp
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f7, -1, "fincstp", NULL, -1, -1, 0, -1},
    // fnstcw
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xd9, 7, "fnstcw", NULL, -1, -1, 0, -1},
    // fstcw
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0x9bd9, 7, "fstcw", NULL, -1, -1, 0, -1},
    // fprem
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f8, -1, "fprem", NULL, -1, -1, 0, -1},
    // fyl2xp1
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9f9, -1, "fyl2xp1", NULL, -1, -1, 0, -1},
    // fsqrt
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9fa, -1, "fsqrt", NULL, -1, -1, 0, -1},
    // fsincos
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9fb, -1, "fsincos", NULL, -1, -1, 0, -1},
    // frndint
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9fc, -1, "frndint", NULL, -1, -1, 0, -1},
    // fscale
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9fd, -1, "fscale", NULL, -1, -1, 0, -1},
    // fsin
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9fe, -1, "fsin", NULL, -1, -1, 0, -1},
    // fcos
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xd9ff, -1, "fcos", NULL, -1, -1, 0, -1},
    // fiadd
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 0, "fiadd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 0, "fiadd", NULL, -1, -1, 0, -1},
    // fcmovb
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 0, "fcmovb", NULL, -1, -1, 0, -1},
    // fimul
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 1, "fimul", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 1, "fimul", NULL, -1, -1, 0, -1},
    // fcmove
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 1, "fcmove", NULL, -1, -1, 0, -1},
    // ficom
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 2, "ficom", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &floatmem_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 2, "ficom", NULL, -1, -1, 0, -1},
    // fcmovbe
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 2, "fcmovbe", NULL, -1, -1, 0, -1},
    // ficomp
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 3, "ficomp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 3, "ficomp", NULL, -1, -1, 0, -1},
    // fcmovu
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 3, "fcmovu", NULL, -1, -1, 0, -1},
    // fisub
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 4, "fisub", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 4, "fisub", NULL, -1, -1, 0, -1},
    // fisubr
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 5, "fisubr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 5, "fisubr", NULL, -1, -1, 0, -1},
    // fucompp
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdae9, -1, "fucompp", NULL, -1, -1, 0, -1},
    // fidiv
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 6, "fidiv", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 6, "fidiv", NULL, -1, -1, 0, -1},
    // fidivr
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xda, 7, "fidivr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 7, "fidivr", NULL, -1, -1, 0, -1},
    // fild
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 0, "fild", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 0, "fild", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 5, "fild", NULL, -1, -1, 0, -1},
    // fcmovnb
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 0, "fcmovnb", NULL, -1, -1, 0, -1},
    // fisttp
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xdb, 1, "fisttp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xdd, 1, "fisttp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xdf, 1, "fisttp", NULL, -1, -1, 0, -1},
    // fcmovne
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 1, "fcmovne", NULL, -1, -1, 0, -1},
    // fist
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 2, "fist", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 2, "fist", NULL, -1, -1, 0, -1},
    // fcmovnbe
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 2, "fcmovnbe", NULL, -1, -1, 0, -1},
    // fistp
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 3, "fistp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 7, "fistp", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 3, "fistp", NULL, -1, -1, 0, -1},
    // fneni
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdbe0, -1, "fneni,feni", NULL, -1, -1, 0, -1},
    // fndisi
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdbe1, -1, "fndisi,fdisi", NULL, -1, -1, 0, -1},
    // fnclex
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdbe2, -1, "fnclex", NULL, -1, -1, 0, -1},
    // fclex
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0x9bdbe2, -1, "fclex", NULL, -1, -1, 0, -1},
    // fninit
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdbe3, -1, "fninit", NULL, -1, -1, 0, -1},
    // finit
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0x9bdbe3, -1, "finit", NULL, -1, -1, 0, -1},
    // fnsetpm
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdbe4, -1, "fnsetpm", NULL, -1, -1, 0, -1},
    // fcmovnu
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 3, "fcmovnu", NULL, -1, -1, 0, -1},
    // fucomi
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 5, "fucomi", NULL, -1, -1, 0, -1},
    // fcomi
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdb, 6, "fcomi", NULL, -1, -1, 0, -1},
    // ffree
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 0, "ffree", NULL, -1, -1, 0, -1},
    // frstor
    {0, 0, 0, 0, 0, &memory_memory_108, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 4, "frstor", NULL, -1, -1, 0, -1},
    // fucom
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 4, "fucom", NULL, -1, -1, 0, -1},
    // fucomp
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 5, "fucomp", NULL, -1, -1, 0, -1},
    // fnsave
    {0, 0, 0, 0, 0, &memory_memory_108, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 6, "fnsave", NULL, -1, -1, 0, -1},
    // fsave
    {0, 0, 0, 0, 0, &memory_memory_108, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0x9bdd, 6, "fsave", NULL, -1, -1, 0, -1},
    // fnstsw
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdd, 7, "fnstsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_word_ax, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xdfe0, -1, "fnstsw", NULL, -1, -1, 0, -1},
    // fstsw
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0x9bdd, 7, "fstsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_word_ax, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0x9bdfe0, -1, "fstsw", NULL, -1, -1, 0, -1},
    // faddp
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 0, "faddp", NULL, -1, -1, 0, -1},
    // fmulp
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 1, "fmulp", NULL, -1, -1, 0, -1},
    // fcompp
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, FLOAT_INSTRUCTION, false, -1, 0xded9, -1, "fcompp", NULL, -1, -1, 0, -1},
    // fsubrp
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 4, "fsubrp", NULL, -1, -1, 0, -1},
    // fsubp
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 5, "fsubp", NULL, -1, -1, 0, -1},
    // fdivrp
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 6, "fdivrp", NULL, -1, -1, 0, -1},
    // fdivp
    {0, 0, 0, 0, 0, &memfloat, &float_register_st0, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xde, 7, "fdivp", NULL, -1, -1, 0, -1},
    // ffreep
    {0, 0, 0, 0, 0, &memfloat, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 0, "ffreep", NULL, -1, -1, 0, -1},
    // fbld
    {0, 0, 0, 0, 0, &memory_memory_tbyte, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 4, "fbld", NULL, -1, -1, 0, -1},
    // fucomip
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 5, "fucomip", NULL, -1, -1, 0, -1},
    // fcomip
    {0, 0, 0, 0, 0, &float_register_st0, &memfloat, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 6, "fcomip", NULL, -1, -1, 0, -1},
    // fbstp
    {0, 0, 0, 0, 0, &memory_memory_tbyte, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0xdf, 6, "fbstp", NULL, -1, -1, 0, -1},
    // sldt
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 0, "sldt", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 0, "sldt", NULL, -1, -1, 0, -1},
    // str
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 1, "str", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 1, "str", NULL, -1, -1, 0, -1},
    // lldt
    {0, 0, 0, 0, 0, &memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 2, "lldt", NULL, -1, -1, 0, -1},
    // ltr
    {0, 0, 0, 0, 0, &memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 3, "ltr", NULL, -1, -1, 0, -1},
    // verr
    {0, 0, 0, 0, 0, &memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 4, "verr", NULL, -1, -1, 0, -1},
    // verw
    {0, 0, 0, 0, 0, &memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f00, 5, "verw", NULL, -1, -1, 0, -1},
    // vmcall
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, VMX_INSTRUCTION, false, -1, 0x0f01c1, -1, "vmcall", NULL, -1, -1, 0, -1},
    // vmlaunch
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, VMX_INSTRUCTION, false, -1, 0x0f01c2, -1, "vmlaunch", NULL, -1, -1, 0, -1},
    // vmresume
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, VMX_INSTRUCTION, false, -1, 0x0f01c3, -1, "vmresume", NULL, -1, -1, 0, -1},
    // vmxoff
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, VMX_INSTRUCTION, false, -1, 0x0f01c4, -1, "vmxoff", NULL, -1, -1, 0, -1},
    // monitor
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SSE3_INSTRUCTION, false, -1, 0x0f01c8, -1, "monitor", NULL, -1, -1, 0, -1},
    // mwait
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SSE3_INSTRUCTION, false, -1, 0x0f01c9, -1, "mwait", NULL, -1, -1, 0, -1},
    // xgetbv
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f01d0, -1, "xgetbv", NULL, -1, -1, 0, -1},
    // xsetbv
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f01d1, -1, "xsetbv", NULL, -1, -1, 0, -1},
    // rdtscp
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f01f9, -1, "rdtscp", NULL, -1, -1, 0, -1},
    // sgdt
    {0, 0, 0, 0, 0, &memory_memory_fixed_fword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 0, "sgdt", NULL, -1, -1, 0, -1},
    // sidt
    {0, 0, 0, 0, 0, &memory_memory_fixed_fword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 1, "sidt", NULL, -1, -1, 0, -1},
    // lgdt
    {0, 0, 0, 0, 0, &memory_memory_fixed_fword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 2, "lgdt", NULL, -1, -1, 0, -1},
    // lidt
    {0, 0, 0, 0, 0, &memory_memory_fixed_fword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 3, "lidt", NULL, -1, -1, 0, -1},
    // smsw
    {0, 0, 0, 0, 0, &memory_memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 4, "smsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 4, "smsw", NULL, -1, -1, 0, -1},
    // lmsw
    {0, 0, 0, 0, 0, &memory_word, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 6, "lmsw", NULL, -1, -1, 0, -1},
    // invlpg
    {0, 0, 0, 0, 0, &memory_memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f01, 7, "invlpg", NULL, -1, -1, 0, -1},
    // lar
    {0, 0, 0, 0, 0, &general_register_dword, &memory_word, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f02, -1, "lar", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f02, -1, "lar", NULL, -1, -1, 0, -1},
    // lsl
    {0, 0, 0, 0, 0, &general_register_dword, &memory_word, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f03, -1, "lsl", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f03, -1, "lsl", NULL, -1, -1, 0, -1},
    // syscall
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f05, -1, "syscall", NULL, -1, -1, 0, -1},
    // clts
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f06, -1, "clts", NULL, -1, -1, 0, -1},
    // sysret
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f07, -1, "sysret", NULL, -1, -1, 0, -1},
    // invd
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f08, -1, "invd", NULL, -1, -1, 0, -1},
    // wbinvd
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f09, -1, "wbinvd", NULL, -1, -1, 0, -1},
    // ud2
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f0b, -1, "ud2", NULL, -1, -1, 0, -1},
    // movups
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f10, -1, "movups", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_dqword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f11, -1, "movups", NULL, -1, -1, 0, -1},
    // movss
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f10, -1, "movss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_fixed_dword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f11, -1, "movss", NULL, -1, -1, 0, -1},
    // movupd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f10, -1, "movupd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f11, -1, "movupd", NULL, -1, -1, 0, -1},
    // movsd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f10, -1, "movsd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_fixed_qword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f11, -1, "movsd", NULL, -1, -1, 0, -1},
    // movhlps
    {0, 0, 0, 0, 0, &sse_register, &memsse, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f12, -1, "movhlps", NULL, -1, -1, 0, -1},
    // movlps
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f12, -1, "movlps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f13, -1, "movlps", NULL, -1, -1, 0, -1},
    // movlpd
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f12, -1, "movlpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f13, -1, "movlpd", NULL, -1, -1, 0, -1},
    // movddup
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf20f12, -1, "movddup", NULL, -1, -1, 0, -1},
    // movsldup
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf30f12, -1, "movsldup", NULL, -1, -1, 0, -1},
    // unpcklps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f14, -1, "unpcklps", NULL, -1, -1, 0, -1},
    // unpcklpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f14, -1, "unpcklpd", NULL, -1, -1, 0, -1},
    // unpckhps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f15, -1, "unpckhps", NULL, -1, -1, 0, -1},
    // unpckhpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f15, -1, "unpckhpd", NULL, -1, -1, 0, -1},
    // movlhps
    {0, 0, 0, 0, 0, &sse_register, &memsse, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f16, -1, "movlhps", NULL, -1, -1, 0, -1},
    // movhps
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f16, -1, "movhps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f17, -1, "movhps", NULL, -1, -1, 0, -1},
    // movhpd
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f16, -1, "movhpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f17, -1, "movhpd", NULL, -1, -1, 0, -1},
    // movshdup
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf30f16, -1, "movshdup", NULL, -1, -1, 0, -1},
    // prefetch*
    {0, 0, 0, 0, 0, &memory_memory_byte, NULL, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f18, 0, "prefetchnta", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_byte, NULL, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f18, 1, "prefetcht0", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_byte, NULL, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f18, 2, "prefetcht1", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_byte, NULL, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f18, 3, "prefetcht2", NULL, -1, -1, 0, -1},
    // movaps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f28, -1, "movaps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_dqword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f29, -1, "movaps", NULL, -1, -1, 0, -1},
    // movapd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f28, -1, "movapd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f29, -1, "movapd", NULL, -1, -1, 0, -1},
    // cvtpi2ps
    {0, 0, 0, 0, 0, &sse_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f2a, -1, "cvtpi2ps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &memory_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f2a, -1, "cvtsi2ss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &mmxmem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f2a, -1, "cvtpi2pd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &memory_fixed_dword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f2a, -1, "cvtsi2sd", NULL, -1, -1, 0, -1},
    // movntps
    {0, 0, 0, 0, 0, &memory_memory_dqword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f2b, -1, "movntps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f2b, -1, "movntpd", NULL, -1, -1, 0, -1},
    // cvttps2pi
    {0, 0, 0, 0, 0, &mmx_register, &ssemem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f2c, -1, "cvttps2pi", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f2c, -1, "cvttss2si", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f2c, -1, "cvttpd2pi", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f2c, -1, "cvttsd2si", NULL, -1, -1, 0, -1},
    // cvtps2pi
    {0, 0, 0, 0, 0, &mmx_register, &ssemem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f2d, -1, "cvtps2pi", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f2d, -1, "cvtss2si", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f2d, -1, "cvtpd2pi", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f2d, -1, "cvtsd2si", NULL, -1, -1, 0, -1},
    // ucomiss
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f2e, -1, "ucomiss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f2e, -1, "ucomisd", NULL, -1, -1, 0, -1},
    // comiss
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f2f, -1, "comiss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f2f, -1, "comisd", NULL, -1, -1, 0, -1},
    // wrmsr
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f30, -1, "wrmsr", NULL, -1, -1, 0, -1},
    // rdtsc
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f31, -1, "rdtsc", NULL, -1, -1, 0, -1},
    // rdmsr
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f32, -1, "rdmsr", NULL, -1, -1, 0, -1},
    // rdpmc
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f33, -1, "rdpmc", NULL, -1, -1, 0, -1},
    // sysenter
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f34, -1, "sysenter", NULL, -1, -1, 0, -1},
    // sysexit
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0f35, -1, "sysexit", NULL, -1, -1, 0, -1},
    // getsec
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SMX_INSTRUCTION, false, -1, 0x0f37, -1, "getsec", NULL, -1, -1, 0, -1},
    // pshufb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3800, -1, "pshufb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3800, -1, "pshufb", NULL, -1, -1, 0, -1},
    // phaddw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3801, -1, "phaddw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3801, -1, "phaddw", NULL, -1, -1, 0, -1},
    // phaddd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3802, -1, "phaddd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3802, -1, "phaddd", NULL, -1, -1, 0, -1},
    // phaddsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3803, -1, "phaddsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3803, -1, "phaddsw", NULL, -1, -1, 0, -1},
    // pmaddubsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3804, -1, "pmaddubsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3804, -1, "pmaddubsw", NULL, -1, -1, 0, -1},
    // phsubw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3805, -1, "phsubw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3805, -1, "phsubw", NULL, -1, -1, 0, -1},
    // phsubd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3806, -1, "phsubd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3806, -1, "phsubd", NULL, -1, -1, 0, -1},
    // phsubsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3807, -1, "phsubsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3807, -1, "phsubsw", NULL, -1, -1, 0, -1},
    // psignb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3808, -1, "psignb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3808, -1, "psignb", NULL, -1, -1, 0, -1},
    // psignw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3809, -1, "psignw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3809, -1, "psignw", NULL, -1, -1, 0, -1},
    // psignd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f380a, -1, "psignd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f380a, -1, "psignd", NULL, -1, -1, 0, -1},
    // pmulhrsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f380b, -1, "pmulhrsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f380b, -1, "pmulhrsw", NULL, -1, -1, 0, -1},
    // pblendvb
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3810, -1, "pblendvb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3814, -1, "blendvps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3815, -1, "blendvpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3817, -1, "ptest", NULL, -1, -1, 0, -1},
    // pabsb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x0f381c, -1, "pabsb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x660f381c, -1, "pabsb", NULL, -1, -1, 0, -1},
    // pabsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x0f381d, -1, "pabsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x660f381d, -1, "pabsw", NULL, -1, -1, 0, -1},
    // pabsd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x0f381e, -1, "pabsd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x660f381e, -1, "pabsd", NULL, -1, -1, 0, -1},
    // pmovsxbw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3820, -1, "pmovsxbw", NULL, -1, -1, 0, -1},
    // pmovsxbd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3821, -1, "pmovsxbd", NULL, -1, -1, 0, -1},
    // pmovsxbq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_word, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3822, -1, "pmovsxbq", NULL, -1, -1, 0, -1},
    // pmovsxwd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3823, -1, "pmovsxwd", NULL, -1, -1, 0, -1},
    // pmovsxwq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3824, -1, "pmovsxwq", NULL, -1, -1, 0, -1},
    // pmovsxdq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3825, -1, "pmovsxdq", NULL, -1, -1, 0, -1},
    // pmuldq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3828, -1, "pmuldq", NULL, -1, -1, 0, -1},
    // pcmpeqq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3829, -1, "pcmpeqq", NULL, -1, -1, 0, -1},
    // movntdqa
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f382a, -1, "movntdqa", NULL, -1, -1, 0, -1},
    // packusdw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f382b, -1, "packusdw", NULL, -1, -1, 0, -1},
    // pmovzxbw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3830, -1, "pmovzxbw", NULL, -1, -1, 0, -1},
    // pmovzxbd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3831, -1, "pmovzxbd", NULL, -1, -1, 0, -1},
    // pmovzxbq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_word, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3832, -1, "pmovzxbq", NULL, -1, -1, 0, -1},
    // pmovzxwd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3833, -1, "pmovzxwd", NULL, -1, -1, 0, -1},
    // pmovzxwq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3834, -1, "pmovzxwq", NULL, -1, -1, 0, -1},
    // pmovzxdq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3835, -1, "pmovzxdq", NULL, -1, -1, 0, -1},
    // pcmpgtq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE42_INSTRUCTION, true, -1, 0x660f3837, -1, "pcmpgtq", NULL, -1, -1, 0, -1},
    // pminsb
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3838, -1, "pminsb", NULL, -1, -1, 0, -1},
    // pminsd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3839, -1, "pminsd", NULL, -1, -1, 0, -1},
    // pminuw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f383a, -1, "pminuw", NULL, -1, -1, 0, -1},
    // pminud
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f383b, -1, "pminud", NULL, -1, -1, 0, -1},
    // pmaxsb
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f383c, -1, "pmaxsb", NULL, -1, -1, 0, -1},
    // pmaxsd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f383d, -1, "pmaxsd", NULL, -1, -1, 0, -1},
    // pmaxuw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f383e, -1, "pmaxuw", NULL, -1, -1, 0, -1},
    // pmaxud
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f383f, -1, "pmaxud", NULL, -1, -1, 0, -1},
    // pmulld
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3840, -1, "pmulld", NULL, -1, -1, 0, -1},
    // phminposuw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3841, -1, "phminposuw", NULL, -1, -1, 0, -1},
    // invept
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &memory_memory_dqword, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x660f3880, -1, "invept", NULL, -1, -1, 0, -1},
    // invvpid
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &memory_memory_dqword, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x660f3881, -1, "invvpid", NULL, -1, -1, 0, -1},
    // movbe
    {0, 0, 0, 0, 0, &general_register_dword, &memory_memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f38f0, -1, "movbe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f38f1, -1, "movbe", NULL, -1, -1, 0, -1},
    // crc32
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &memory_byte, NULL, NULL, SSE42_INSTRUCTION, true, -1, 0xf20f38f0, -1, "crc32", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &memory_dword, NULL, NULL, SSE42_INSTRUCTION, true, -1, 0xf20f38f1, -1, "crc32", NULL, -1, -1, 0, -1},
    // roundps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a08, -1, "roundps", NULL, -1, -1, 0, -1},
    // roundpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a09, -1, "roundpd", NULL, -1, -1, 0, -1},
    // roundss
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a0a, -1, "roundss", NULL, -1, -1, 0, -1},
    // roundsd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a0b, -1, "roundsd", NULL, -1, -1, 0, -1},
    // blendps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a0c, -1, "blendps", NULL, -1, -1, 0, -1},
    // blendpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a0d, -1, "blendpd", NULL, -1, -1, 0, -1},
    // pblendw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a0e, -1, "pblendw", NULL, -1, -1, 0, -1},
    // palignr
    {0, 0, 0, 0, 0, &mmx_register, &ssemem_fixed_qword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x0f3a0f, -1, "palignr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSSE3_INSTRUCTION, true, -1, 0x660f3a0f, -1, "palignr", NULL, -1, -1, 0, -1},
    // pextrb
    {0, 0, 0, 0, 0, &memory_memory_byte, &sse_register, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a14, -1, "pextrb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &sse_register, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a14, -1, "pextrb", NULL, -1, -1, 0, -1},
    // pextrw
    {0, 0, 0, 0, 0, &memory_memory_word, &sse_register, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a15, -1, "pextrw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &sse_register, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a15, -1, "pextrw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &mmx_register, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x0fc5, -1, "pextrw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &sse_register, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x660fc5, -1, "pextrw", NULL, -1, -1, 0, -1},
    // pextrd
    {0, 0, 0, 0, 0, &memory_fixed_dword, &sse_register, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a16, -1, "pextrd", NULL, -1, -1, 0, -1},
    // extractps
    {0, 0, 0, 0, 0, &memory_fixed_dword, &sse_register, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a17, -1, "extractps", NULL, -1, -1, 0, -1},
    // pinsrb
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_byte, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a20, -1, "pinsrb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &memory_register_fixed_dword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a20, -1, "pinsrb", NULL, -1, -1, 0, -1},
    // insertps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a21, -1, "insertps", NULL, -1, -1, 0, -1},
    // pinsrd
    {0, 0, 0, 0, 0, &sse_register, &memory_fixed_dword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a22, -1, "pinsrd", NULL, -1, -1, 0, -1},
    // dpps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a40, -1, "dpps", NULL, -1, -1, 0, -1},
    // dppd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a41, -1, "dppd", NULL, -1, -1, 0, -1},
    // mpsadbw
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE41_INSTRUCTION, true, -1, 0x660f3a42, -1, "mpsadbw", NULL, -1, -1, 0, -1},
    // pcmpestrm
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE42_INSTRUCTION, true, -1, 0x660f3a60, -1, "pcmpestrm", NULL, -1, -1, 0, -1},
    // pcmpestri
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE42_INSTRUCTION, true, -1, 0x660f3a61, -1, "pcmpestri", NULL, -1, -1, 0, -1},
    // pcmpistrm
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE42_INSTRUCTION, true, -1, 0x660f3a62, -1, "pcmpistrm", NULL, -1, -1, 0, -1},
    // pcmpistri
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE42_INSTRUCTION, true, -1, 0x660f3a63, -1, "pcmpistri", NULL, -1, -1, 0, -1},
    // cmov
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f40, -1, "cmovo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f41, -1, "cmovno", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f42, -1, "cmovb,cmovnae,cmovc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f43, -1, "cmovnb,cmovae,cmovnc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f44, -1, "cmovz,cmove", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f45, -1, "cmovnz,cmovne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f46, -1, "cmovbe,cmovna", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f47, -1, "cmovnbe,cmova", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f48, -1, "cmovs", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f49, -1, "cmovns", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f4a, -1, "cmovp,cmovpe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f4b, -1, "cmovnp,cmovpo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f4c, -1, "cmovl,cmovnge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f4d, -1, "cmovnl,cmovge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f4e, -1, "cmovle,cmovng", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f4f, -1, "cmovnle,cmovg", NULL, -1, -1, 0, -1},
    // movmskps
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f50, -1, "movmskps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f50, -1, "movmskpd", NULL, -1, -1, 0, -1},
    // sqrtps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f51, -1, "sqrtps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f51, -1, "sqrtss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f51, -1, "sqrtpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f51, -1, "sqrtsd", NULL, -1, -1, 0, -1},
    // rsqrtps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f52, -1, "rsqrtps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f52, -1, "rsqrtss", NULL, -1, -1, 0, -1},
    // rcpps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f53, -1, "rcpps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f53, -1, "rcpss", NULL, -1, -1, 0, -1},
    // andps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f54, -1, "andps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f54, -1, "andpd", NULL, -1, -1, 0, -1},
    // andnps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f55, -1, "andnps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f55, -1, "andnpd", NULL, -1, -1, 0, -1},
    // orps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f56, -1, "orps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f56, -1, "orpd", NULL, -1, -1, 0, -1},
    // xorps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f57, -1, "xorps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f57, -1, "xorpd", NULL, -1, -1, 0, -1},
    // addps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f58, -1, "addps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f58, -1, "addss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f58, -1, "addpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f58, -1, "addsd", NULL, -1, -1, 0, -1},
    // mulps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f59, -1, "mulps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f59, -1, "mulss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f59, -1, "mulpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f59, -1, "mulsd", NULL, -1, -1, 0, -1},
    // cvtps2pd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x0f5a, -1, "cvtps2pd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30f5a, -1, "cvtss2sd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f5a, -1, "cvtpd2ps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f5a, -1, "cvtsd2ss", NULL, -1, -1, 0, -1},
    // cvtdq2ps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x0f5b, -1, "cvtdq2ps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30f5b, -1, "cvtps2dq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f5b, -1, "cvttps2dq", NULL, -1, -1, 0, -1},
    // subps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f5c, -1, "subps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f5c, -1, "subss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f5c, -1, "subpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f5c, -1, "subsd", NULL, -1, -1, 0, -1},
    // minps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f5d, -1, "minps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f5d, -1, "minss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f5d, -1, "minpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f5d, -1, "minsd", NULL, -1, -1, 0, -1},
    // divps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f5e, -1, "divps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f5e, -1, "divss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f5e, -1, "divpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f5e, -1, "divsd", NULL, -1, -1, 0, -1},
    // maxps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0f5f, -1, "maxps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0xf30f5f, -1, "maxss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f5f, -1, "maxpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f5f, -1, "maxsd", NULL, -1, -1, 0, -1},
    // punpcklbw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f60, -1, "punpcklbw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f60, -1, "punpcklbw", NULL, -1, -1, 0, -1},
    // punpcklwd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f61, -1, "punpcklwd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f61, -1, "punpcklwd", NULL, -1, -1, 0, -1},
    // punpckldq
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f62, -1, "punpckldq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f62, -1, "punpckldq", NULL, -1, -1, 0, -1},
    // packsswb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f63, -1, "packsswb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f63, -1, "packsswb", NULL, -1, -1, 0, -1},
    // pcmpgtb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f64, -1, "pcmpgtb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f64, -1, "pcmpgtb", NULL, -1, -1, 0, -1},
    // pcmpgtw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f65, -1, "pcmpgtw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f65, -1, "pcmpgtw", NULL, -1, -1, 0, -1},
    // pcmpgtd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f66, -1, "pcmpgtd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f66, -1, "pcmpgtd", NULL, -1, -1, 0, -1},
    // packuswb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f67, -1, "packuswb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f67, -1, "packuswb", NULL, -1, -1, 0, -1},
    // punpckhbw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f68, -1, "punpckhbw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f68, -1, "punpckhbw", NULL, -1, -1, 0, -1},
    // punpckhwd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f69, -1, "punpckhwd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f69, -1, "punpckhwd", NULL, -1, -1, 0, -1},
    // punpckhdq
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f6a, -1, "punpckhdq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f6a, -1, "punpckhdq", NULL, -1, -1, 0, -1},
    // packssdw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f6b, -1, "packssdw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f6b, -1, "packssdw", NULL, -1, -1, 0, -1},
    // punpcklqdq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f6c, -1, "punpcklqdq", NULL, -1, -1, 0, -1},
    // punpckhqdq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f6d, -1, "punpckhqdq", NULL, -1, -1, 0, -1},
    // movd
    {0, 0, 0, 0, 0, &mmx_register, &memory_dword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f6e, -1, "movd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &memory_dword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f6e, -1, "movd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_fixed_dword, &mmx_register, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f7e, -1, "movd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_fixed_dword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f7e, -1, "movd", NULL, -1, -1, 0, -1},
    // movq
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f6f, -1, "movq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30f7e, -1, "movq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmxmem_fixed_qword, &mmx_register, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f7f, -1, "movq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_fixed_qword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd6, -1, "movq", NULL, -1, -1, 0, -1},
    // movdqa
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f6f, -1, "movdqa", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &ssemem_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f7f, -1, "movdqa", NULL, -1, -1, 0, -1},
    // movdqu
    {0, 0, 0, 0, 0, &ssemem_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30f7f, -1, "movdqu", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30f6f, -1, "movdqu", NULL, -1, -1, 0, -1},
    // pshufw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x0f70, -1, "pshufw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE2_INSTRUCTION, true, -1, 0x660f70, -1, "pshufd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE2_INSTRUCTION, true, -1, 0xf30f70, -1, "pshufhw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE2_INSTRUCTION, true, -1, 0xf20f70, -1, "pshuflw", NULL, -1, -1, 0, -1},
    // psrlw
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f71, 2, "psrlw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f71, 2, "psrlw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fd1, -1, "psrlw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd1, -1, "psrlw", NULL, -1, -1, 0, -1},
    // psraw
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f71, 4, "psraw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f71, 4, "psraw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fe1, -1, "psraw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe1, -1, "psraw", NULL, -1, -1, 0, -1},
    // psllw
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f71, 6, "psllw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f71, 6, "psllw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ff1, -1, "psllw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff1, -1, "psllw", NULL, -1, -1, 0, -1},
    // psrld
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f72, 2, "psrld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f72, 2, "psrld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fd2, -1, "psrld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd2, -1, "psrld", NULL, -1, -1, 0, -1},
    // psrad
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f72, 4, "psrad", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f72, 4, "psrad", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fe2, -1, "psrad", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe2, -1, "psrad", NULL, -1, -1, 0, -1},
    // pslld
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f72, 6, "pslld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f72, 6, "pslld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ff2, -1, "pslld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff2, -1, "pslld", NULL, -1, -1, 0, -1},
    // psrlq
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f73, 2, "psrlq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f73, 2, "psrlq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fd3, -1, "psrlq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd3, -1, "psrlq", NULL, -1, -1, 0, -1},
    // psrldq
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f73, 3, "psrldq", NULL, -1, -1, 0, -1},
    // psllq
    {0, 0, 0, 0, 0, &memmmx, &immediate_byte, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f73, 6, "psllq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f73, 6, "psllq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ff3, -1, "psllq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff3, -1, "psllq", NULL, -1, -1, 0, -1},
    // pslldq
    {0, 0, 0, 0, 0, &memsse, &immediate_byte, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f73, 7, "pslldq", NULL, -1, -1, 0, -1},
    // pcmpeqb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f74, -1, "pcmpeqb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f74, -1, "pcmpeqb", NULL, -1, -1, 0, -1},
    // pcmpeqw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f75, -1, "pcmpeqw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f75, -1, "pcmpeqw", NULL, -1, -1, 0, -1},
    // pcmpeqd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0f76, -1, "pcmpeqd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660f76, -1, "pcmpeqd", NULL, -1, -1, 0, -1},
    // emms
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, MMX_INSTRUCTION, false, -1, 0x0f77, -1, "emms", NULL, -1, -1, 0, -1},
    // vmread
    {0, 0, 0, 0, 0, &memory_fixed_dword, &general_register_fixed_dword, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x0f78, -1, "vmread", NULL, -1, -1, 0, -1},
    // vmwrite
    {0, 0, 0, 0, 0, &general_register_fixed_dword, &memory_fixed_dword, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x0f79, -1, "vmwrite", NULL, -1, -1, 0, -1},
    // haddpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x660f7c, -1, "haddpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf20f7c, -1, "haddps", NULL, -1, -1, 0, -1},
    // hsubpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x660f7d, -1, "hsubpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf20f7d, -1, "hsubps", NULL, -1, -1, 0, -1},
    // set
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f90, 0, "seto", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f91, 0, "setno", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f92, 0, "setb,setnae,setc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f93, 0, "setnb,setae,setnc", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f94, 0, "setz,sete", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f95, 0, "setnz,setne", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f96, 0, "setbe,setna", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f97, 0, "setnbe,seta", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f98, 0, "sets", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f99, 0, "setns", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f9a, 0, "setp,setpe", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f9b, 0, "setnp,setpo", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f9c, 0, "setl,setnge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f9d, 0, "setnl,setge", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f9e, 0, "setle,setng", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_byte, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0f9f, 0, "setnle,setg", NULL, -1, -1, 0, -1},
    // fxsave
    {0, 0, 0, 0, 0, &memory_memory_512, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0x0fae, 0, "fxsave", NULL, -1, -1, 0, -1},
    // fxrstor
    {0, 0, 0, 0, 0, &memory_memory_512, NULL, NULL, NULL, FLOAT_INSTRUCTION, true, -1, 0x0fae, 1, "fxrstor", NULL, -1, -1, 0, -1},
    // ldmxcsr
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fae, 2, "ldmxcsr", NULL, -1, -1, 0, -1},
    // stmxcsr
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fae, 3, "stmxcsr", NULL, -1, -1, 0, -1},
    // xsave
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fae, 4, "xsave", NULL, -1, -1, 0, -1},
    // lfence
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SSE2_INSTRUCTION, false, -1, 0x0faee8, -1, "lfence", NULL, -1, -1, 0, -1},
    // xrstor
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fae, 5, "xrstor", NULL, -1, -1, 0, -1},
    // mfence
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SSE2_INSTRUCTION, false, -1, 0x0faef0, -1, "mfence", NULL, -1, -1, 0, -1},
    // sfence
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, SSE1_INSTRUCTION, false, -1, 0x0faef8, -1, "sfence", NULL, -1, -1, 0, -1},
    // clflush
    {0, 0, 0, 0, 0, &memory_memory_byte, NULL, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x0fae, 7, "clflush", NULL, -1, -1, 0, -1},
    // cpuid
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fa2, -1, "cpuid", NULL, -1, -1, 0, -1},
    // shrd
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, &immediate_byte, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fac, -1, "shrd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, &general_register_byte_cl_default, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fad, -1, "shrd", NULL, -1, -1, 0, -1},
    // cmpxchg
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb0, -1, "cmpxchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb1, -1, "cmpxchg", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_fixed_qword, NULL, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fc7, 1, "cmpxchg8b", NULL, -1, -1, 0, -1},
    // lss
    {0, 0, 0, 0, 0, &general_register_dword, &memory_fword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb2, -1, "lss", NULL, -1, -1, 0, -1},
    // btr
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb3, -1, "btr", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fba, 6, "btr", NULL, -1, -1, 0, -1},
    // lfs
    {0, 0, 0, 0, 0, &general_register_dword, &memory_fword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb4, -1, "lfs", NULL, -1, -1, 0, -1},
    // lgs
    {0, 0, 0, 0, 0, &general_register_dword, &memory_fword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb5, -1, "lgs", NULL, -1, -1, 0, -1},
    // movzx
    {0, 0, 0, 0, 0, &general_register_dword, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb6, -1, "movzx", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_word, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fb7, -1, "movzx", NULL, -1, -1, 0, -1},
    // popcnt
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0xf30fb8, -1, "popcnt", NULL, -1, -1, 0, -1},
    // rsm
    {0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0faa, -1, "rsm", NULL, -1, -1, 0, -1},
    // bt
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fa3, -1, "bt", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fba, 4, "bt", NULL, -1, -1, 0, -1},
    // shld
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, &immediate_byte, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fa4, -1, "shld", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_dword, &general_register_dword, &general_register_byte_cl_default, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fa5, -1, "shld", NULL, -1, -1, 0, -1},
    // bts
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fab, -1, "bts", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fba, 5, "bts", NULL, -1, -1, 0, -1},
    // btc
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &immediate_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fba, 7, "btc", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fbb, -1, "btc", NULL, -1, -1, 0, -1},
    // bsf
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fbc, -1, "bsf", NULL, -1, -1, 0, -1},
    // bsr
    {0, 0, 0, 0, 0, &general_register_dword, &memory_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fbd, -1, "bsr", NULL, -1, -1, 0, -1},
    // movsx
    {0, 0, 0, 0, 0, &general_register_dword, &memory_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fbe, -1, "movsx", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword, &memory_word, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fbf, -1, "movsx", NULL, -1, -1, 0, -1},
    // xadd
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_byte, &general_register_byte, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fc0, -1, "xadd", NULL, -1, -1, 0, -1},
    {LOCK_PERMIT, 0, 0, 0, 0, &memory_dword, &general_register_dword, NULL, NULL, NORMAL_INSTRUCTION, true, -1, 0x0fc1, -1, "xadd", NULL, -1, -1, 0, -1},
    // cmpps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x0fc2, -1, "cmpps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_dword, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0xf30fc2, -1, "cmpss", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE2_INSTRUCTION, true, -1, 0x660fc2, -1, "cmppd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_fixed_qword, &immediate_byte, NULL, SSE2_INSTRUCTION, true, -1, 0xf20fc2, -1, "cmpsd", NULL, -1, -1, 0, -1},
    // movnti
    {0, 0, 0, 0, 0, &memory_memory_fixed_dword, &general_register_fixed_dword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x0fc3, -1, "movnti", NULL, -1, -1, 0, -1},
    // pinsrw
    {0, 0, 0, 0, 0, &mmx_register, &memory_register_fixed_dword, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x0fc4, -1, "pinsrw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &mmx_register, &memory_memory_word, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x0fc4, -1, "pinsrw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &memory_register_fixed_dword, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x660fc4, -1, "pinsrw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_word, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x660fc4, -1, "pinsrw", NULL, -1, -1, 0, -1},
    // shufps
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE1_INSTRUCTION, true, -1, 0x0fc6, -1, "shufps", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, &immediate_byte, NULL, SSE2_INSTRUCTION, true, -1, 0x660fc6, -1, "shufpd", NULL, -1, -1, 0, -1},
    // vmptrld
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x0fc7, 6, "vmptrld", NULL, -1, -1, 0, -1},
    // vmclear
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x660fc7, 6, "vmclear", NULL, -1, -1, 0, -1},
    // vmxon
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, VMX_INSTRUCTION, true, -1, 0xf30fc7, 6, "vmxon", NULL, -1, -1, 0, -1},
    // vmptrst
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, NULL, NULL, NULL, VMX_INSTRUCTION, true, -1, 0x0fc7, 7, "vmptrst", NULL, -1, -1, 0, -1},
    // bswap
    {0, 0, 0, 0, 0, &general_register_dword_eax, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fc8, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ecx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fc9, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fca, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebx, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fcb, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esp, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fcc, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_ebp, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fcd, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_esi, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fce, -1, "bswap", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &general_register_dword_edi, NULL, NULL, NULL, NORMAL_INSTRUCTION, false, -1, 0x0fcf, -1, "bswap", NULL, -1, -1, 0, -1},
    // addsubpd
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0x660fd0, -1, "addsubpd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf20fd0, -1, "addsubps", NULL, -1, -1, 0, -1},
    // paddq
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fd4, -1, "paddq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd4, -1, "paddq", NULL, -1, -1, 0, -1},
    // pmullw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x0fd5, -1, "pmullw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd5, -1, "pmullw", NULL, -1, -1, 0, -1},
    // movq2dq
    {0, 0, 0, 0, 0, &memsse, &mmx_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30fd6, -1, "movq2dq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memmmx, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20fd6, -1, "movq2dq", NULL, -1, -1, 0, -1},
    // pmovmskb
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &mmx_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fd7, -1, "pmovmskb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_register_fixed_dword, &sse_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fd7, -1, "pmovmskb", NULL, -1, -1, 0, -1},
    // psubusb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fd8, -1, "psubusb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd8, -1, "psubusb", NULL, -1, -1, 0, -1},
    // psubusw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fd9, -1, "psubusw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fd9, -1, "psubusw", NULL, -1, -1, 0, -1},
    // pminub
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fda, -1, "pminub", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fda, -1, "pminub", NULL, -1, -1, 0, -1},
    // pand
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fdb, -1, "pand", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fdb, -1, "pand", NULL, -1, -1, 0, -1},
    // paddusb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fdc, -1, "paddusb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fdc, -1, "paddusb", NULL, -1, -1, 0, -1},
    // paddusw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fdd, -1, "paddusw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fdd, -1, "paddusw", NULL, -1, -1, 0, -1},
    // pmaxub
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fde, -1, "pmaxub", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fde, -1, "pmaxub", NULL, -1, -1, 0, -1},
    // pandn
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fdf, -1, "pandn", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fdf, -1, "pandn", NULL, -1, -1, 0, -1},
    // pavgb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fe0, -1, "pavgb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fe0, -1, "pavgb", NULL, -1, -1, 0, -1},
    // pavgw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fe3, -1, "pavgw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fe3, -1, "pavgw", NULL, -1, -1, 0, -1},
    // pmulhuw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fe4, -1, "pmulhuw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fe4, -1, "pmulhuw", NULL, -1, -1, 0, -1},
    // pmulhw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fe5, -1, "pmulhw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe5, -1, "pmulhw", NULL, -1, -1, 0, -1},
    // cvtpd2dq
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf20fe6, -1, "cvtpd2dq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe6, -1, "cvttpd2dq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0xf30fe6, -1, "cvtdq2pd", NULL, -1, -1, 0, -1},
    // movntq
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, &mmx_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fe7, -1, "movntq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe7, -1, "movntdq", NULL, -1, -1, 0, -1},
    // psubsb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fe8, -1, "psubsb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe8, -1, "psubsb", NULL, -1, -1, 0, -1},
    // psubsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fe9, -1, "psubsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fe9, -1, "psubsw", NULL, -1, -1, 0, -1},
    // pminsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fea, -1, "pminsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fea, -1, "pminsw", NULL, -1, -1, 0, -1},
    // por
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0feb, -1, "por", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660feb, -1, "por", NULL, -1, -1, 0, -1},
    // paddsb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fec, -1, "paddsb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fec, -1, "paddsb", NULL, -1, -1, 0, -1},
    // paddsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fed, -1, "paddsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fed, -1, "paddsw", NULL, -1, -1, 0, -1},
    // pmaxsw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0fee, -1, "pmaxsw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x660fee, -1, "pmaxsw", NULL, -1, -1, 0, -1},
    // pxor
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0fef, -1, "pxor", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660fef, -1, "pxor", NULL, -1, -1, 0, -1},
    // lddqu
    {0, 0, 0, 0, 0, &sse_register, &memory_memory_dqword, NULL, NULL, SSE3_INSTRUCTION, true, -1, 0xf20ff0, -1, "lddqu", NULL, -1, -1, 0, -1},
    // pmuludq
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x0ff4, -1, "pmuludq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff4, -1, "pmuludq", NULL, -1, -1, 0, -1},
    // pmaddwd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ff5, -1, "pmaddwd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff5, -1, "pmaddwd", NULL, -1, -1, 0, -1},
    // psadbw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0ff6, -1, "psadbw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff6, -1, "psadbw", NULL, -1, -1, 0, -1},
    // maskmovq
    {0, 0, 0, 0, 0, &memory_memory_fixed_qword, &mmx_register, NULL, NULL, SSE1_INSTRUCTION, true, -1, 0x0ff7, -1, "maskmovq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &memory_memory_dqword, &sse_register, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff7, -1, "maskmovdqu", NULL, -1, -1, 0, -1},
    // psubb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ff8, -1, "psubb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff8, -1, "psubb", NULL, -1, -1, 0, -1},
    // psubw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ff9, -1, "psubw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ff9, -1, "psubw", NULL, -1, -1, 0, -1},
    // psubd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ffa, -1, "psubd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ffa, -1, "psubd", NULL, -1, -1, 0, -1},
    // psubq
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ffb, -1, "psubq", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ffb, -1, "psubq", NULL, -1, -1, 0, -1},
    // paddb
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ffc, -1, "paddb", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ffc, -1, "paddb", NULL, -1, -1, 0, -1},
    // paddw
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ffd, -1, "paddw", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ffd, -1, "paddw", NULL, -1, -1, 0, -1},
    // paddd
    {0, 0, 0, 0, 0, &mmx_register, &mmxmem_fixed_qword, NULL, NULL, MMX_INSTRUCTION, true, -1, 0x0ffe, -1, "paddd", NULL, -1, -1, 0, -1},
    {0, 0, 0, 0, 0, &sse_register, &ssemem_dqword, NULL, NULL, SSE2_INSTRUCTION, true, -1, 0x660ffe, -1, "paddd", NULL, -1, -1, 0, -1},
};
