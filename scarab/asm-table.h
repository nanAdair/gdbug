/* @(#)asm-table.h
 */

#ifndef _ASM_TABLE_H
#define _ASM_TABLE_H 1

#include "asm.h"

#define PREFIX_NUM            7
#define GENERAL_REGISTER_NUM  24
#define FLOAT_REGISTER_NUM    8
#define SSEMMX_REGISTER_NUM   16
#define SIZE_NUM              16
#define SEGMENT_NUM           7
#define SPECIAL_NUM           1
#define CR_NUM                8
#define DR_NUM                8
#define MNEMONIC_NUM          1075

#define REPN_PERMIT           1
#define REP_PERMIT            2
#define LOCK_PERMIT           4

extern const char *g_prefix_lr[];
extern const char *g_general_register[];
extern const char *g_float_register[][8];
extern const char *g_ssemmx_register[];
extern const char *g_size[];
extern const char *g_segment[];
extern const char *g_special[];
extern const char *g_cr[];
extern const char *g_dr[];
extern ASMINSTRUCTION g_instr[];

#endif /* _ASM-TABLE_H */
