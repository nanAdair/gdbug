#include "disasm.h"
#include "dis-table.h"

const char *RepeatPrefixes[] = {"lock", "rep", "repe", "repne"};
const char *SegmentRegisters[] = {"es", "cs", "ss", "ds", "fs", "gs", "seg?", "seg?"};
const char *Register32[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "ebx + al"};
const char *Register16[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "bx + al"};
const char *Register8[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *Register16_ModRM[] = {"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx", "bx + al", "ax", "cx", "dx", "sp"};
const char *FloatPointRegister[] = {"st(0)", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)"};
const char *ControlRegister[] = {"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"};
const char *DebugRegister[] = {"dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7"};
const char *XMMRegister[] = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};
const char *MMRegister[] = {"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7"};
const char *PTR[] = {"word ptr ", "dword ptr ", "word ptr ", "byte ptr "};

Disasm::Disasm()
{
    currentByte = 0;
    codeSize = 0;
    address = 0;
    base = 0;
}

INT8 Disasm::decodePrefix()
{
    currentByte++;
    switch((unsigned char)machineCode[currentByte - 1]){
    case PREFIX_WAIT:
	if (waitPrefix != -1)
	    return NOT_PREFIX;
	waitPrefix = PREFIX_WAIT;
	return NOT_PREFIX;
    case PREFIX_LOCK:
	if (lockAndRepeat != -1)
	    return COLLIDED_PREFIX;
	lockAndRepeat = PREFIX_LOCK;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_REPN:
	if (lockAndRepeat != -1)
	    return COLLIDED_PREFIX;
	lockAndRepeat = PREFIX_REPN;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_REP:
	if (lockAndRepeat != -1)
	    return COLLIDED_PREFIX;
	lockAndRepeat = PREFIX_REP;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_CS:
	if (segmentOverride != -1)
	    return COLLIDED_PREFIX;
	segmentOverride = CS;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_SS:
	if (segmentOverride != -1)
	    return COLLIDED_PREFIX;
	segmentOverride = SS;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_DS:
	if (segmentOverride != -1)
	    return COLLIDED_PREFIX;
	segmentOverride = DS;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_ES:
	if (segmentOverride != -1)
	    return COLLIDED_PREFIX;
	segmentOverride = ES;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_FS:
	if (segmentOverride != -1)
	    return COLLIDED_PREFIX;
	segmentOverride = FS;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_GS:
	if (segmentOverride != -1)
	    return COLLIDED_PREFIX;
	segmentOverride = GS;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_OPERAND:
	if (OperandSizeOverride != -1)
	    return COLLIDED_PREFIX;
	OperandSizeOverride = PREFIX_OPERAND;
	return CONTINUE_DECODING_PREFIX;
    case PREFIX_ADDRESS:
	if (AddressSizeOverride != -1)
	    return COLLIDED_PREFIX;
	AddressSizeOverride = PREFIX_ADDRESS;
	return CONTINUE_DECODING_PREFIX;
    default:
	currentByte--;
    }
    return NOT_PREFIX;
}

void Disasm::preprocessingFloatPointInstruction()
{
    INT8 mod = (machineCode[currentByte] >> 6) & 3;
    INT8 rm = (machineCode[currentByte]) & 7;
    INT8 regop = (machineCode[currentByte] >> 3) & 7;
    INT32 opcode = instruction->opcode;
    currentByte++;

    switch(opcode){
    case 0xd8:
    {
	if (mod != 3)
	    break;
	memcpy(instruction, &floatInstruction_2[FLOAT_d8][regop & 7], sizeof(INSTRUCTION));
	break;
    }
    case 0xd9:
    {
	switch((unsigned char)regop){
	case 0:
	{
	    if (mod == 3)
		memcpy(instruction, &floatInstruction_2[FLOAT_d9][regop & 7], sizeof(INSTRUCTION));
	    break;
	}
	case 1:
	{
	    if (mod == 3)
		memcpy(instruction, &floatInstruction_2[FLOAT_d9][regop & 7], sizeof(INSTRUCTION));
	    break;
	}
	case 2:
	{
	    if (mod != 3)
		break;
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xd0)
		memcpy(instruction, &specialInstruction[SPECIAL_d9d0], sizeof(INSTRUCTION));
	    else
		memcpy(instruction, &floatInstruction_2[FLOAT_d9][regop & 7], sizeof(INSTRUCTION));
	    break;
	}
	case 3:
	{
	    if (mod == 3)
		memcpy(instruction, &floatInstruction_2[FLOAT_d9][regop & 7], sizeof(INSTRUCTION));
	    break;
	}
	case 4:
	{
	    if ((unsigned char)machineCode[currentByte - 1] == 0xe0)
		memcpy(instruction, &specialInstruction[SPECIAL_d9e0], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xe1)
		memcpy(instruction, &specialInstruction[SPECIAL_d9e1], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xe4)
		memcpy(instruction, &specialInstruction[SPECIAL_d9e4], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xe5)
		memcpy(instruction, &specialInstruction[SPECIAL_d9e5], sizeof(INSTRUCTION));
	    else if (mod == 3)
		memcpy(instruction, &floatInstruction_2[FLOAT_d9][regop & 7], sizeof(INSTRUCTION));
	    break;
	}
	case 5:
	{
	    if ((unsigned char)machineCode[currentByte - 1] == 0xe8)
		memcpy(instruction, &specialInstruction[SPECIAL_d9e8], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xe9)
		memcpy(instruction, &specialInstruction[SPECIAL_d9e9], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xea)
		memcpy(instruction, &specialInstruction[SPECIAL_d9ea], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xeb)
		memcpy(instruction, &specialInstruction[SPECIAL_d9eb], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xec)
		memcpy(instruction, &specialInstruction[SPECIAL_d9ec], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xed)
		memcpy(instruction, &specialInstruction[SPECIAL_d9ed], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xee)
		memcpy(instruction, &specialInstruction[SPECIAL_d9ee], sizeof(INSTRUCTION));
	    else if (mod == 3)
		memcpy(instruction, &floatInstruction_2[FLOAT_d9][regop & 7], sizeof(INSTRUCTION));
	    break;
	}
	case 6:
	{
	    if (waitPrefix != -1){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bd9_1], sizeof(INSTRUCTION));
	    }
	    else{
		if ((unsigned char)machineCode[currentByte - 1] == 0xf0)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f0], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf1)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f1], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf2)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f2], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf3)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f3], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf4)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f4], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf5)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f5], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf6)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f6], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf7)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f7], sizeof(INSTRUCTION));
	    }
	    break;
	}
	case 7:
	{
	    if (waitPrefix != -1){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bd9_2], sizeof(INSTRUCTION));
	    }
	    else{
		if ((unsigned char)machineCode[currentByte - 1] == 0xf8)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f8], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xf9)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9f9], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xfa)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9fa], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xfb)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9fb], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xfc)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9fc], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xfd)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9fd], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xfe)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9fe], sizeof(INSTRUCTION));
		else if ((unsigned char)machineCode[currentByte - 1] == 0xff)
		    memcpy(instruction, &specialInstruction[SPECIAL_d9ff], sizeof(INSTRUCTION));
	    }
	    break;
	}
	}
	break;
    }
    case 0xda:
    {
	if (mod != 3)
	    break;
	if ((unsigned char)machineCode[currentByte - 1] == 0xe9)
	    memcpy(instruction, &specialInstruction[SPECIAL_dae9], sizeof(INSTRUCTION));
	else
	    memcpy(instruction, &floatInstruction_2[FLOAT_da][regop & 7], sizeof(INSTRUCTION));
	break;
    }
    case 0xdb:
    {
	if ((unsigned char)machineCode[currentByte - 1] == 0xe0)
	    memcpy(instruction, &specialInstruction[SPECIAL_dbe0], sizeof(INSTRUCTION));
	else if ((unsigned char)machineCode[currentByte - 1] == 0xe1)
	    memcpy(instruction, &specialInstruction[SPECIAL_dbe1], sizeof(INSTRUCTION));
	else if ((unsigned char)machineCode[currentByte - 1] == 0xe2){
	    if (waitPrefix != -1){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bdbe2], sizeof(INSTRUCTION));
	    }
	    else
		memcpy(instruction, &specialInstruction[SPECIAL_dbe2], sizeof(INSTRUCTION));
	}
	else if ((unsigned char)machineCode[currentByte - 1] == 0xe3){
	    if (waitPrefix != -1){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bdbe3], sizeof(INSTRUCTION));
	    }
	    else
		memcpy(instruction, &specialInstruction[SPECIAL_dbe3], sizeof(INSTRUCTION));
	}
	else if ((unsigned char)machineCode[currentByte - 1] == 0xe4)
	    memcpy(instruction, &specialInstruction[SPECIAL_dbe4], sizeof(INSTRUCTION));
	else{
	    if (mod != 3)
		break;
	    memcpy(instruction, &floatInstruction_2[FLOAT_db][regop & 7], sizeof(INSTRUCTION));
	}
	break;
    }
    case 0xdc:
    {
	if (mod != 3)
	    break;
	memcpy(instruction, &floatInstruction_2[FLOAT_dc][regop & 7], sizeof(INSTRUCTION));
	break;
    }
    case 0xdd:
    {
	if (waitPrefix != -1){
	    if ((unsigned char)regop == 6){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bdd_1], sizeof(INSTRUCTION));
	    }
	    else if ((unsigned char)regop == 7){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bdd_2], sizeof(INSTRUCTION));
	    }
	    if (waitPrefix == -1 && mod == 3)
		memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	}
	else if (mod == 3)
	    memcpy(instruction, &floatInstruction_2[FLOAT_dd][regop & 7], sizeof(INSTRUCTION));
	break;
    }
    case 0xde:
    {
	if (mod != 3)
	    break;
	if ((unsigned char)machineCode[currentByte - 1] == 0xd9)
	    memcpy(instruction, &specialInstruction[SPECIAL_ded9], sizeof(INSTRUCTION));
	else
	    memcpy(instruction, &floatInstruction_2[FLOAT_de][regop & 7], sizeof(INSTRUCTION));
	break;
    }
    case 0xdf:
    {
	if (waitPrefix != -1){
	    if ((unsigned char)machineCode[currentByte - 1] == 0xe0){
		waitPrefix = -1;
		memcpy(instruction, &specialInstruction[SPECIAL_9bdfe0], sizeof(INSTRUCTION));
	    }
	}
	else{
	    if (mod != 3)
		break;
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xe0)
		memcpy(instruction, &specialInstruction[SPECIAL_dfe0], sizeof(INSTRUCTION));
	    else
		memcpy(instruction, &floatInstruction_2[FLOAT_df][regop & 7], sizeof(INSTRUCTION));
	}
	break;
    }
    default:
	break;
    }
    instruction->rm = rm;
    instruction->regop = regop;
    instruction->mod = mod;
    instruction->opcode = opcode;

    if ((instruction->dest && (instruction->dest->type == OPERAND_MEMORY || instruction->dest->type == OPERAND_MEMORY_MEMORY)) ||
	(instruction->src1 && (instruction->src1->type == OPERAND_MEMORY || instruction->src1->type == OPERAND_MEMORY_MEMORY)) ||
	(instruction->src2 && (instruction->src2->type == OPERAND_MEMORY || instruction->src2->type == OPERAND_MEMORY_MEMORY)) ||
	(instruction->src3 && (instruction->src3->type == OPERAND_MEMORY || instruction->src3->type == OPERAND_MEMORY_MEMORY)) ||
	instruction->type == UNDEFINED_INSTRUCTION)
	currentByte--;
}

INT8 Disasm::specialMnemonicProcess()
{
    INT32 opcode = instruction->opcode;
    switch(opcode){
    // PUSHAD
    case 0x60:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_60], sizeof(INSTRUCTION));
	instruction->type = STACK_OPERATE_INSTRUCTION;
	break;
    }
    // POPAD
    case 0x61:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_61], sizeof(INSTRUCTION));
	instruction->type = STACK_OPERATE_INSTRUCTION;
	break;
    }
    // PUSHFD
    case 0x9c:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_9c], sizeof(INSTRUCTION));
	instruction->type = STACK_OPERATE_INSTRUCTION;
	break;
    }
    // POPFD
    case 0x9d:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_9d], sizeof(INSTRUCTION));
	instruction->type = STACK_OPERATE_INSTRUCTION;
	break;
    }
    // NOP
    case 0x90:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_90_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_90_2], sizeof(INSTRUCTION));
	else
	    instruction->type = IRREPLACEABLE_INSTRUCTION;
	break;
    }
	// CBW
	case 0x98:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_98], sizeof(INSTRUCTION));
	    instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	// CDQ
	case 0x99:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_99], sizeof(INSTRUCTION));
	    instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	    // IRET
	case 0xcf:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_cf], sizeof(INSTRUCTION));
	    instruction->type = STACK_OPERATE_INSTRUCTION;
	    break;
	}
	    // AAM
	case 0xd4:{
	    if ((unsigned char)machineCode[currentByte] == 0x0a){
		currentByte++;
		memcpy(instruction, &specialInstruction[SPECIAL_d4], sizeof(INSTRUCTION));
	    }
	    instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	    // AAD
	case 0xd5:{
	    if ((unsigned char)machineCode[currentByte] == 0x0a){
		currentByte++;
		memcpy(instruction, &specialInstruction[SPECIAL_d5], sizeof(INSTRUCTION));
	    }
	    instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	case 0xe0:
	    {
		if (AddressSizeOverride == PREFIX_ADDRESS)
		    memcpy(instruction, &specialInstruction[SPECIAL_e0], sizeof(INSTRUCTION));
		instruction->type = FLOW_INSTRUCTION;
		break;
	    }
	case 0xe1:
	    {
		if (AddressSizeOverride == PREFIX_ADDRESS)
		    memcpy(instruction, &specialInstruction[SPECIAL_e1], sizeof(INSTRUCTION));
		instruction->type = FLOW_INSTRUCTION;
		break;
	    }
	case 0xe2:
	    {
		if (AddressSizeOverride == PREFIX_ADDRESS)
		    memcpy(instruction, &specialInstruction[SPECIAL_e2], sizeof(INSTRUCTION));
		instruction->type = FLOW_INSTRUCTION;
		break;
	    }
	case 0xe3:
	    {
		if (AddressSizeOverride == PREFIX_ADDRESS)
		    memcpy(instruction, &specialInstruction[SPECIAL_e3], sizeof(INSTRUCTION));
		instruction->type = FLOW_INSTRUCTION;
		break;
	    }
	case 0x0f00:{
	    if (((machineCode[currentByte] >> 6) & 3) == 3 && ((machineCode[currentByte] >> 3) & 7) == 0)
		memcpy(instruction, &specialInstruction[SPECIAL_0f00_1], sizeof(INSTRUCTION));
	    else if (((machineCode[currentByte] >> 6) & 3) == 3 && ((machineCode[currentByte] >> 3) & 7) == 1)
		memcpy(instruction, &specialInstruction[SPECIAL_0f00_2], sizeof(INSTRUCTION));
	    else
		instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	case 0x0f01:{
	    currentByte++;

	    if ((unsigned char)machineCode[currentByte - 1] == 0xc1)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01c1], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xc2)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01c2], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xc3)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01c3], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xc4)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01c4], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xc8)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01c8], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xc9)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01c9], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xd0)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01d0], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xd1)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01d1], sizeof(INSTRUCTION));
	    else if ((unsigned char)machineCode[currentByte - 1] == 0xf9)
		memcpy(instruction, &specialInstruction[SPECIAL_0f01f9], sizeof(INSTRUCTION));
	    else if (((machineCode[currentByte - 1] >> 3) & 7) == 0x4 && ((machineCode[currentByte - 1] >> 6) & 3) == 0x3){
		currentByte--;
		memcpy(instruction, &specialInstruction[SPECIAL_0f10_4], sizeof(INSTRUCTION));
	    }
	    else{
		instruction->type = NORMAL_INSTRUCTION;
		currentByte--;
	    }
	    break;
	}
	case 0x0f02:{
	    INT8 mod = (machineCode[currentByte] >> 6) & 3;
	    if (mod == 3)
		memcpy(instruction, &specialInstruction[SPECIAL_0f02_1], sizeof(INSTRUCTION));
	    instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	case 0x0f03:{
	    INT8 mod = (machineCode[currentByte] >> 6) & 3;
	    if (mod == 3)
		memcpy(instruction, &specialInstruction[SPECIAL_0f03_1], sizeof(INSTRUCTION));
	    instruction->type = NORMAL_INSTRUCTION;
	    break;
	}
	case 0x0fb8:{
	    if ((unsigned char)lockAndRepeat != PREFIX_REP)
		memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	    else{
		lockAndRepeat = -1;
		instruction->type = NORMAL_INSTRUCTION;
	    }
	    break;
	}
	case 0x0fc7:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0fc7_1], sizeof(INSTRUCTION));
	    else if ((unsigned char)lockAndRepeat == PREFIX_REP){
		memcpy(instruction, &specialInstruction[SPECIAL_0fc7_2], sizeof(INSTRUCTION));
		lockAndRepeat = -1;
	    }
	    else
		instruction->type = VMX_INSTRUCTION;
	    break;
	}
	default:
	    break;
    }
    instruction->opcode = opcode;
    return DECODE_SUCCESS;
}

INT8 Disasm::SSEAndMMXInstructionProcess(){
    switch(instruction->opcode){
	case 0x0f10:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f10_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f10_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f10_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f11:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f11_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f11_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f11_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f12:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f12_1], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN && ((machineCode[currentByte] >> 6) & 3) != 3)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f12_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REP && ((machineCode[currentByte] >> 6) & 3) != 3)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f12_3], sizeof(INSTRUCTION));
	else if (((machineCode[currentByte] >> 6) & 3) == 3)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f12_4], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f13: case 0x0f14: case 0x0f15:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f13 + instruction->opcode - 0x0f13], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f16:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f16_1], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REP && ((machineCode[currentByte] >> 6) & 3) != 3)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f16_2], sizeof(INSTRUCTION));
	else if (((machineCode[currentByte] >> 6) & 3) == 3)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f16_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f17:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f17], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f28:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f28], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f29:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f29], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f2a:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2a_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2a_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2a_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f2b:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2b], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f2c:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2c_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2c_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2c_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f2d:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2d_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2d_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2d_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f2e:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2e_1], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f2f:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f2f_1], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f38:{
	if (currentByte == codeSize)
	    return NOT_ENOUGH_CODE;
	switch((unsigned char)machineCode[currentByte++]){
	case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05:
	case 0x06: case 0x07: case 0x08: case 0x09: case 0x0a: case 0x0b:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3800_2 + machineCode[currentByte - 1] * 2], sizeof(INSTRUCTION));
	    else
		memcpy(instruction, &specialInstruction[SPECIAL_0f3800_1 + machineCode[currentByte - 1] * 2], sizeof(INSTRUCTION));
	    break;
	}
	case 0x10:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3810_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x14:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3814_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x15:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3815_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x17:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3817_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x1c: case 0x1d: case 0x1e:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f381c_2 + (machineCode[currentByte - 1] - 0x1c) * 2], sizeof(INSTRUCTION));
	    else
		memcpy(instruction, &specialInstruction[SPECIAL_0f381c_1 + (machineCode[currentByte - 1] - 0x1c) * 2], sizeof(INSTRUCTION));
	    break;
	}
	case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3820_1 + machineCode[currentByte - 1] - 0x20], sizeof(INSTRUCTION));
	    break;
	}
	case 0x28: case 0x29: case 0x2a: case 0x2b:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3828_1 + machineCode[currentByte - 1] - 0x28], sizeof(INSTRUCTION));
	    break;
	}
	case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3830_1 + machineCode[currentByte - 1] - 0x30], sizeof(INSTRUCTION));
	    break;
	}
	case 0x37: case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d:
	case 0x3e: case 0x3f: case 0x40: case 0x41:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3837_1 + machineCode[currentByte - 1] - 0x37], sizeof(INSTRUCTION));
	    break;
	}
	case 0x80:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3880_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x81:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3881_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0xf0:{
	    INT8 mod = (machineCode[currentByte] >> 6) & 3;
	    if ((unsigned char)lockAndRepeat == PREFIX_REPN){
		memcpy(instruction, &specialInstruction[SPECIAL_0f38f0_2], sizeof(INSTRUCTION));
		lockAndRepeat = -1;
	    }
	    else if (mod != 3)
		memcpy(instruction, &specialInstruction[SPECIAL_0f38f0_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0xf1:{
	    INT8 mod = (machineCode[currentByte] >> 6) & 3;
	    if ((unsigned char)lockAndRepeat == PREFIX_REPN){
		memcpy(instruction, &specialInstruction[SPECIAL_0f38f1_2], sizeof(INSTRUCTION));
		lockAndRepeat = -1;
	    }
	    else if (mod != 3)
		memcpy(instruction, &specialInstruction[SPECIAL_0f38f1_1], sizeof(INSTRUCTION));
	    break;
	}
	default:{
	    break;
	}
	}
	if (instruction->instr_class == UNDEFINED_CLASS){
	    instruction->type = UNDEFINED_INSTRUCTION;
	    currentByte--;
	}
	break;
    }
    case 0x0f3a:{
	if (currentByte == codeSize)
	    return NOT_ENOUGH_CODE;
	switch((unsigned char)machineCode[currentByte++]){
	case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a08_1 + machineCode[currentByte - 1] - 0x08], sizeof(INSTRUCTION));
	    break;
	}
	case 0x0f:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a0f_2], sizeof(INSTRUCTION));
	    else
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a0f_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x14: case 0x15:{
	    if (OperandSizeOverride == PREFIX_OPERAND){
		INT8 mod = (machineCode[currentByte] >> 6) & 3;
		if (mod == 3)
		    memcpy(instruction, &specialInstruction[SPECIAL_0f3a14_2 + (machineCode[currentByte - 1] - 0x14) * 2], sizeof(INSTRUCTION));
		else
		    memcpy(instruction, &specialInstruction[SPECIAL_0f3a14_1 + (machineCode[currentByte - 1] - 0x14) * 2], sizeof(INSTRUCTION));
	    }
	    break;
	}
	case 0x16:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a16_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x17:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a17_1], sizeof(INSTRUCTION));
	    break;
	}
	case 0x20:{
	    if (OperandSizeOverride == PREFIX_OPERAND){
		if (((machineCode[currentByte] >> 6) & 3) == 3)
		    memcpy(instruction, &specialInstruction[SPECIAL_0f3a20_2 + (machineCode[currentByte - 1] - 0x20) * 2], sizeof(INSTRUCTION));
		else
		    memcpy(instruction, &specialInstruction[SPECIAL_0f3a20_1 + (machineCode[currentByte - 1] - 0x20) * 2], sizeof(INSTRUCTION));
	    }
	    break;
	}
	case 0x21: case 0x22:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a21_1 + machineCode[currentByte - 1] - 0x21], sizeof(INSTRUCTION));
	    break;
	}
	case 0x40: case 0x41: case 0x42:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a40_1 + machineCode[currentByte - 1] - 0x40], sizeof(INSTRUCTION));
	    break;
	}
	case 0x60: case 0x61: case 0x62: case 0x63:{
	    if (OperandSizeOverride == PREFIX_OPERAND)
		memcpy(instruction, &specialInstruction[SPECIAL_0f3a60_1 + machineCode[currentByte - 1] - 0x60], sizeof(INSTRUCTION));
	    break;
	}
	default:
	    break;
	}
	if (instruction->instr_class == UNDEFINED_CLASS){
	    instruction->type = UNDEFINED_INSTRUCTION;
	    currentByte--;
	}
	break;
    }
    case 0x0f50:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f50_1], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f51:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f51_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f51_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f51_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f52: case 0x0f53:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP){
	    memcpy(instruction, &specialInstruction[SPECIAL_0f52_1 + instruction->opcode - 0x0f52], sizeof(INSTRUCTION));
	    lockAndRepeat = -1;
	}
	break;
    }
    case 0x0f54: case 0x0f55: case 0x0f56: case 0x0f57:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f54_1 + instruction->opcode - 0x0f54], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f58: case 0x0f59: case 0x0f5a:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f58_1 + (instruction->opcode - 0x0f58) * 3], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f58_2 + (instruction->opcode - 0x0f58) * 3], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f58_3 + (instruction->opcode - 0x0f58) * 3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f5b:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP){
	    memcpy(instruction, &specialInstruction[SPECIAL_0f5b_1], sizeof(INSTRUCTION));
	    lockAndRepeat = -1;
	}
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f5b_2], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f5c: case 0x0f5d: case 0x0f5e: case 0x0f5f:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f5c_1 + (instruction->opcode - 0x0f5c) * 3], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f5c_2 + (instruction->opcode - 0x0f5c) * 3], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f5c_3 + (instruction->opcode - 0x0f5c) * 3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f60: case 0x0f61: case 0x0f62: case 0x0f63: case 0x0f64: case 0x0f65:
    case 0x0f66: case 0x0f67: case 0x0f68: case 0x0f69: case 0x0f6a: case 0x0f6b:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f60_1 + instruction->opcode - 0x0f60], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f6c: case 0x0f6d:{
	if (OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f6e:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f6e_1], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f6f:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f6f_1], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REP){
	    memcpy(instruction, &specialInstruction[SPECIAL_0f6f_2], sizeof(INSTRUCTION));
	    lockAndRepeat = -1;
	}
	break;
    }
    case 0x0f70:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f70_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f70_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f70_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0f71: case 0x0f72: case 0x0f73:{
	int auxiliaryOpcode = (machineCode[currentByte] >> 3) & 0x7;
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &getOneByteWithAuxiliaryOpcode[AUXILIARY_0f71_2 + instruction->opcode - 0x0f71][auxiliaryOpcode], sizeof(INSTRUCTION));
	if (instruction->type == SSE1_INSTRUCTION)
	    instruction->type = UNDEFINED_INSTRUCTION;
	break;
    }
    case 0x0f74: case 0x0f75: case 0x0f76:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f74_1 + instruction->opcode - 0x0f74], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f7c: case 0x0f7d:{
	if ((unsigned char)lockAndRepeat == PREFIX_REPN && OperandSizeOverride != PREFIX_OPERAND){
	    memcpy(instruction, &specialInstruction[SPECIAL_0f7d_1 + instruction->opcode - 0x0f7d], sizeof(INSTRUCTION));
	    lockAndRepeat = -1;
	}
	else if (OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	break;
    }
    case 0x0f7e: case 0x0f7f:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0f7e_1 + (instruction->opcode - 0x0f7e) * 2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REP){
	    memcpy(instruction, &specialInstruction[SPECIAL_0f7e_2 + (instruction->opcode - 0x0f7e) * 2], sizeof(INSTRUCTION));
	    lockAndRepeat = -1;
	}
	break;
    }
    case 0x0fae:{
	INT8 mod = (machineCode[currentByte] >> 6) & 3;
	INT8 regop = (machineCode[currentByte] >> 3) & 7;

	if (mod == 3){
	    memcpy(instruction, &getOneByteWithAuxiliaryOpcode[AUXILIARY_0fae_2][regop & 7], sizeof(INSTRUCTION));
	    if (instruction->type != UNDEFINED_INSTRUCTION)
		currentByte++;
	}
	else if (regop == 1 || regop == 5)
	    instruction->type = FLOAT_INSTRUCTION;
	else if (regop == 4)
	    instruction->type = NORMAL_INSTRUCTION;
	else if (regop == 6)
	    currentByte++;
	break;
    }
    case 0x0fc2:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fc2_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fc2_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fc2_3], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0fc4:{
	if (OperandSizeOverride == PREFIX_OPERAND){
	    if (((machineCode[currentByte] >> 6) & 3) != 3)
		memcpy(instruction, &specialInstruction[SPECIAL_0fc4_3], sizeof(INSTRUCTION));
	    else
		memcpy(instruction, &specialInstruction[SPECIAL_0fc4_2], sizeof(INSTRUCTION));
	}
	else if (((machineCode[currentByte] >> 6) & 3) != 3)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fc4_1], sizeof(INSTRUCTION));
	break;
    }
    case 0x0fc5: case 0x0fc6:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fc5_1 + instruction->opcode - 0x0fc5], sizeof(INSTRUCTION));
	break;
    }
    case 0x0fd0:{
	if ((unsigned char)lockAndRepeat == PREFIX_REPN && OperandSizeOverride != PREFIX_OPERAND){
	    memcpy(instruction, &specialInstruction[SPECIAL_0fd0_1], sizeof(INSTRUCTION));
	    lockAndRepeat = -1;
	}
	if (OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	break;
    }
    case 0x0fd1: case 0x0fd2: case 0x0fd3: case 0x0fd4: case 0x0fd5:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fd1_1 + instruction->opcode - 0x0fd1], sizeof(INSTRUCTION));
	break;
    }
    case 0x0fd6:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP && OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fd6_1], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN && OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fd6_2], sizeof(INSTRUCTION));
	else if (OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0fd7: case 0x0fd8: case 0x0fd9: case 0x0fda: case 0x0fdb: case 0x0fdc:
    case 0x0fdd: case 0x0fde: case 0x0fdf: case 0x0fe0: case 0x0fe1: case 0x0fe2:
    case 0x0fe3: case 0x0fe4: case 0x0fe5:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fd7_1 + instruction->opcode - 0x0fd7], sizeof(INSTRUCTION));
	break;
    }
    case 0x0fe6:{
	if ((unsigned char)lockAndRepeat == PREFIX_REP && OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fe6_2], sizeof(INSTRUCTION));
	else if ((unsigned char)lockAndRepeat == PREFIX_REPN && OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fe6_1], sizeof(INSTRUCTION));
	else if (OperandSizeOverride != PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	lockAndRepeat = -1;
	break;
    }
    case 0x0fe7: case 0x0fe8: case 0x0fe9: case 0x0fea: case 0x0feb:
    case 0x0fec: case 0x0fed: case 0x0fee: case 0x0fef:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0fe7_1 + instruction->opcode - 0x0fe7], sizeof(INSTRUCTION));
	break;
    }
    case 0x0ff0:{
	if ((unsigned char)lockAndRepeat != PREFIX_REPN)
	    memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	else
	    lockAndRepeat = -1;
	break;
    }
    case 0x0ff1: case 0x0ff2: case 0x0ff3: case 0x0ff4: case 0x0ff5: case 0x0ff6: case 0x0ff7:
    case 0x0ff8: case 0x0ff9: case 0x0ffa: case 0x0ffb: case 0x0ffc: case 0x0ffd: case 0x0ffe:{
	if (OperandSizeOverride == PREFIX_OPERAND)
	    memcpy(instruction, &specialInstruction[SPECIAL_0ff1_1 + instruction->opcode - 0x0ff1], sizeof(INSTRUCTION));
	break;
    }
    default:
	break;
    }
    return DECODE_SUCCESS;
}

INT8 Disasm::validatePrefix(){
    if (waitPrefix != -1){
	currentByte = hasPrefix + 1;
	memcpy(instruction, &getOneByteOpcode[(unsigned char)0x9b], sizeof(INSTRUCTION));
	instruction->opcode = 0x9b;
    }

    if ((unsigned char)lockAndRepeat == PREFIX_LOCK){
	switch(instruction->instr_class){
	case CLASS_ADD: case CLASS_ADC: case CLASS_AND: case CLASS_BTC:
	case CLASS_BTR: case CLASS_BTS: case CLASS_CMPXCHG: case CLASS_CMPXCHG8B:
	case CLASS_DEC: case CLASS_INC: case CLASS_NEG: case CLASS_NOT:
	case CLASS_OR: case CLASS_SBB: case CLASS_SUB: case CLASS_XOR:
	case CLASS_XADD: case CLASS_XCHG:
	    if (instruction->type == NORMAL_INSTRUCTION && instruction->dest != NULL &&
		instruction->dest->type == OPERAND_MEMORY && ((machineCode[currentByte] >> 6) & 0x3) != 3)
		lockAndRepeat = LOCK;
	    else
		lockAndRepeat = -1;
	    break;
	default:
	    lockAndRepeat = -1;
	    break;
	}
    }
    else if ((unsigned char)lockAndRepeat == PREFIX_REP){
	switch (instruction->opcode) {
	case 0x6c: case 0x6d: case 0xa4: case 0xa5: case 0x6e: case 0x6f: case 0xac: case 0xad:
	case 0xaa: case 0xab:
	    lockAndRepeat = REP;
	    break;
	case 0x90:
	    lockAndRepeat = -1;
	    break;
	case 0xa6: case 0xa7: case 0xae: case 0xaf:
	    lockAndRepeat = REPE;
	    break;
	default:
	    return PREFIX_NOT_QUALIFY;
	}
    }
    else if (lockAndRepeat != -1){
	switch (instruction->opcode) {
	case 0xa6: case 0xa7: case 0xae: case 0xaf:
	    lockAndRepeat = REPNE;
	    break;
	default:
	    return PREFIX_NOT_QUALIFY;
	}
    }

    return PREFIX_QUALIFY;
}

INT8 Disasm::decodeModRM(Operand *operand, INT8 operand_number)
{
    if (operand == NULL)
	return OPERAND_NOT_EXIST;
    INT8 ret = -1;

    if (operand->type == OPERAND_MEMORY || operand->type == OPERAND_MEMORY_OFFSET ||
	operand->type == OPERAND_SSE_MEMORY || operand->type == OPERAND_MMX_MEMORY ||
	operand->type == OPERAND_MEMORY_MEMORY || operand->type == OPERAND_MEMORY_REGISTER ||
	operand->type == OPERAND_MEMORY_SSE || operand->type == OPERAND_MEMORY_MMX){
	currentByte++;
	if (instruction->mod != 3 || operand->type == OPERAND_MEMORY_OFFSET){       // memory
	    if (operand->type == OPERAND_MEMORY_REGISTER || operand->type == OPERAND_MEMORY_SSE ||
		operand->type == OPERAND_MEMORY_MMX){
		if (instruction->dest != NULL)
		    free(instruction->dest);
		if (instruction->src1 != NULL)
		    free(instruction->src1);
		if (instruction->src2 != NULL)
		    free(instruction->src2);
		if (instruction->src3 != NULL)
		    free(instruction->src3);
		memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
		currentByte--;
		return DECODE_SUCCESS;
	    }

	    if (operand->operand_size == SIZE_DWORD){
		if (OperandSizeOverride == PREFIX_OPERAND)
		    operand->operand_size = SIZE_WORD;
	    }
	    else if (operand->operand_size == SIZE_FWORD){
		if (OperandSizeOverride == PREFIX_OPERAND)
		    operand->operand_size = SIZE_DWORD;
	    }
	    else if (operand->operand_size == SIZE_QWORD){
		if (OperandSizeOverride == PREFIX_OPERAND)
		    operand->operand_size = SIZE_DWORD;
	    }
	    else if (operand->operand_size != SIZE_WORD &&
		     operand->operand_size != SIZE_BYTE &&
		     operand->operand_size != SIZE_512BYTE &&
		     operand->operand_size != SIZE_FIXED_DWORD &&
		     operand->operand_size != SIZE_FIXED_FWORD &&
		     operand->operand_size != SIZE_FIXED_QWORD &&
		     operand->operand_size != SIZE_DQWORD &&
		     operand->operand_size != SIZE_TBYTE){
		if (operand->operand_size == SIZE_108BYTE){
		    if (OperandSizeOverride == PREFIX_OPERAND)
			operand->operand_size = SIZE_94BYTE;
		}
		else if (operand->operand_size == SIZE_28BYTE){
		    if (OperandSizeOverride == PREFIX_OPERAND)
			operand->operand_size = SIZE_14BYTE;
		}
	    }

	    if (operand->segment != -1){
		if (segmentOverride != -1 && operand->segment == DS)
		    operand->segment = segmentOverride;
	    }
	    else if (AddressSizeOverride == PREFIX_ADDRESS || instruction->rm != 4){    // don't have sib
		if (AddressSizeOverride == PREFIX_ADDRESS){
		    if (segmentOverride != -1)
			operand->segment = segmentOverride;
		    else if (instruction->rm == 2 || instruction->rm == 3 || (instruction->mod != 0 && instruction->rm == 6))
			operand->segment = SS;
		    else
			operand->segment = DS;
		}
		else{
		    if (segmentOverride != -1)
			operand->segment = segmentOverride;
		    else if (instruction->mod != 0 && instruction->rm == 5)
			operand->segment = SS;
		    else
			operand->segment = DS;
		}
	    }
	    else{           // have sib
		operand->scale  = (machineCode[currentByte] >> 6) & 3;      // first 2 buts of SIB
		operand->base = machineCode[currentByte] & 0x07;   // last 3 bits of SIB
		operand->index = (machineCode[currentByte] >> 3) & 7;  // middle 3 bits of SIB

		if (segmentOverride != -1)
		    operand->segment = segmentOverride;
		else if (operand->base == 4 || (instruction->mod != 0x0 && operand->base == 5))
		    operand->segment = SS;
		else
		    operand->segment = DS;
	    }

	    if (AddressSizeOverride == PREFIX_ADDRESS && operand->addressing_size == SIZE_DWORD)
		operand->addressing_size = SIZE_WORD;

	    if (operand->type == OPERAND_MEMORY_OFFSET){
		currentByte--;
		ret = getImmediateOrDisplacement(operand, IS_OFFSET, operand->addressing_size);
		if (ret == NOT_ENOUGH_CODE)
		    return ret;
	    }
	    else{
		if (operand->operand != -1){          // don't need further decode modr/m or sib
		    currentByte--;                    // don't need next byte to decode
		    if (operand->addressing_size == SIZE_WORD){ // a little adjustment
			if (operand->operand == AX)
			    operand->operand = ADDRESSING_AX;
			else if (operand->operand == CX)
			    operand->operand = ADDRESSING_CX;
			else if (operand->operand == DX)
			    operand->operand = ADDRESSING_DX;
			else if (operand->operand == BX)
			    operand->operand = ADDRESSING_BX;
			else if (operand->operand == SP)
			    operand->operand = ADDRESSING_SP;
			else if (operand->operand == BP)
			    operand->operand = ADDRESSING_BP;
			else if (operand->operand == SI)
			    operand->operand = ADDRESSING_SI;
			else if (operand->operand == DI)
			    operand->operand = ADDRESSING_DI;
		    }
		}
		else{
		    if (operand->addressing_size == SIZE_WORD || instruction->rm != 4){   // don't have sib
			operand->operand = instruction->rm & 7;
			if (instruction->mod == 0){                       // mod = 0
			    if (operand->addressing_size == SIZE_WORD){
				if (instruction->rm == 6){
				    operand->operand = -1;
				    ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_WORD);
				}
			    }
			    else{
				if (instruction->rm == 5){
				    operand->operand = -1;
				    ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_DWORD);
				}
			    }
			    if (ret == NOT_ENOUGH_CODE)
				return ret;
			}
			else if (instruction->mod == 1){            // mod = 1
			    ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_BYTE);
			    if (ret == NOT_ENOUGH_CODE)
				return ret;
			}
			else{               // mod = 2
			    if (operand->addressing_size == SIZE_WORD)
				ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_WORD);
			    else
				ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_DWORD);
			    if (ret == NOT_ENOUGH_CODE)
				return ret;
			}
		    }
		    else{       // need sib
			operand->operand = -1;
			operand->scale  = (machineCode[currentByte] >> 6) & 3;      // first 2 buts of SIB
			operand->base = machineCode[currentByte] & 7;   // last 3 bits of SIB
			operand->index = (machineCode[currentByte] >> 3) & 7;  // middle 3 bits of SIB
			if (codeSize == currentByte)
			    return NOT_ENOUGH_CODE;
			instruction->SIB = machineCode[currentByte++];

			// displacement
			if (instruction->mod == 0){
			    if (operand->base == 5){
				//operand->base = -1;
				ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_DWORD);
			    }
			}
			else if (instruction->mod == 1)
			    ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_BYTE);
			else
			    ret = getImmediateOrDisplacement(operand, IS_DISPLACEMENT, SIZE_DWORD);
			if (ret == NOT_ENOUGH_CODE)
			    return ret;

			// if (operand->index == 0x4){
			//     operand->index = -1;
			//     operand->scale = -1;
			// }
		    }
		}
	    }
	}
	else{      // mod == 3, register
	    if (operand->type == OPERAND_MEMORY_MEMORY){
		if (instruction->dest != NULL)
		    free(instruction->dest);
		if (instruction->src1 != NULL)
		    free(instruction->src1);
		if (instruction->src2 != NULL)
		    free(instruction->src2);
		if (instruction->src3 != NULL)
		    free(instruction->src3);
		currentByte--;
		memcpy(instruction, &specialInstruction[SPECIAL_UNDEFINED], sizeof(INSTRUCTION));
	    }
	    else{
		if (OperandSizeOverride == PREFIX_OPERAND && operand->operand_size == SIZE_DWORD)
		    operand->operand_size = SIZE_WORD;

		operand->operand = instruction->rm & 7;

		if (operand->type == OPERAND_SSE_MEMORY || operand->type == OPERAND_MEMORY_SSE){
		    operand->operand_size = SIZE_XMM;
		    operand->type = OPERAND_SSE_REGISTER;
		}
		else if (operand->type == OPERAND_MMX_MEMORY || operand->type == OPERAND_MEMORY_MMX){
		    operand->operand_size = SIZE_MM;
		    operand->type = OPERAND_MMX_REGISTER;
		}
		else
		    operand->type = OPERAND_REGISTER;
	    }
	}
    }
    else if (operand->type == OPERAND_REGISTER || operand->type == OPERAND_SSE_REGISTER || operand->type == OPERAND_MMX_REGISTER){
	if (operand->operand == -1)
	    operand->operand = instruction->regop & 7;

	if (operand->type == OPERAND_REGISTER){
	    if (operand->operand_size == SIZE_DWORD && OperandSizeOverride == PREFIX_OPERAND)
		operand->operand_size = SIZE_WORD;
	    else if (operand->operand_size == SIZE_QWORD && OperandSizeOverride == PREFIX_OPERAND)
		operand->scale = SIZE_DWORD;
	}
    }
    else if (operand->type == OPERAND_IMMEDIATE){
	if (operand->operand != -1)
	    return DECODE_SUCCESS;
	else if (operand->operand_size == SIZE_BYTE)
	    ret = getImmediateOrDisplacement(operand, IS_IMMEDIATE, operand->operand_size);
	else if (operand->operand_size == SIZE_WORD)
	    ret = getImmediateOrDisplacement(operand, IS_IMMEDIATE, operand->operand_size);
	else if (operand->operand_size == SIZE_FIXED_DWORD)
	    ret = getImmediateOrDisplacement(operand, IS_IMMEDIATE, operand->operand_size);
	else{
	    if (OperandSizeOverride == PREFIX_OPERAND){
		operand->operand_size = SIZE_WORD;
		ret = getImmediateOrDisplacement(operand, IS_IMMEDIATE, operand->operand_size);
	    }
	    else
		ret = getImmediateOrDisplacement(operand, IS_IMMEDIATE, operand->operand_size);
	}
	if (ret == NOT_ENOUGH_CODE)
	    return ret;
    }
    else if (operand->type == OPERAND_SEGMENT){
	if (operand->operand == -1)
	    operand->operand = instruction->regop;
    }
    else if (operand->type == OPERAND_FLOW){
	if (operand->operand_size == RELATIVE_ADDRESS_SHORT){
	    if (codeSize == currentByte)
		return NOT_ENOUGH_CODE;
	    operand->operand = machineCode[currentByte++];
	    instruction->new_eip = address + operand->operand + currentByte;

	    if (OperandSizeOverride == PREFIX_OPERAND)
		instruction->new_eip &= 0x0000ffff;
	}
	else if (operand->operand_size == ABSOLUTE_ADDRESS_FAR){
	    if (OperandSizeOverride == PREFIX_OPERAND)
		operand->operand_size = ABSOLUTE_ADDRESS_FAR_WORD;
	    else
		operand->operand_size = ABSOLUTE_ADDRESS_FAR_DWORD;
	    ret = getImmediateOrDisplacement(operand, IS_NEW_EIP, operand->operand_size);
	    if (ret == NOT_ENOUGH_CODE)
		return ret;

	    ret = getImmediateOrDisplacement(operand, IS_NEW_CS, SIZE_WORD);
	    if (ret == NOT_ENOUGH_CODE)
		return ret;
	}
	else if (operand->operand_size == RELATIVE_ADDRESS_NEAR){
	    if (OperandSizeOverride == PREFIX_OPERAND)
		operand->operand_size = RELATIVE_ADDRESS_NEAR_WORD;
	    else
		operand->operand_size = RELATIVE_ADDRESS_NEAR_DWORD;
	    ret = getImmediateOrDisplacement(operand, IS_NEW_EIP, operand->operand_size);
	    if (ret == NOT_ENOUGH_CODE)
		return ret;

	    instruction->new_eip += address + currentByte;

	    if (OperandSizeOverride == PREFIX_OPERAND)
		instruction->new_eip &= 0x0000ffff;
	}
    }
    else if (operand->type == OPERAND_FLOAT){
	if (operand->operand == -1)
	    operand->operand = instruction->rm;
    }
    else if (operand->type == OPERAND_CR || operand->type == OPERAND_DR){
	if (operand->operand == -1)
	    operand->operand = instruction->regop;
    }
    return DECODE_SUCCESS;
}

INT8 Disasm::getImmediateOrDisplacement(Operand *operand, INT8 type, INT8 size)
{
    INT32 immediate = 0, s = 0;

    if (size == SIZE_BYTE || size == RELATIVE_ADDRESS_SHORT){
	if (codeSize == currentByte)
	    return NOT_ENOUGH_CODE;
	s = 8;
    }
    else if (size == SIZE_WORD || size == ABSOLUTE_ADDRESS_FAR_WORD || size == RELATIVE_ADDRESS_NEAR_WORD){
	if (codeSize < currentByte + 2)
	    return NOT_ENOUGH_CODE;
	s = 16;
    }
    else if (size == SIZE_DWORD || size == ABSOLUTE_ADDRESS_FAR_DWORD || size == RELATIVE_ADDRESS_NEAR_DWORD){
	if (codeSize < currentByte + 4)
	    return NOT_ENOUGH_CODE;
	s = 32;
    }

    memcpy(&immediate, &machineCode[currentByte], s / 8);
    currentByte += s / 8;

    if (type == IS_IMMEDIATE || type == IS_OFFSET)
	operand->operand = immediate;
    else if (type == IS_DISPLACEMENT){
	operand->displacement = immediate;
	operand->displacement_size = size;
    }
    else if (type == IS_NEW_CS)
	instruction->new_cs = immediate;
    else{
	operand->operand = immediate;
	instruction->new_eip = immediate;
    }

    return ADD_SUCCESS;
}

bool Disasm::needToChangeSign(Operand *operand, int size, INT8 type)
{
    if (instruction->s != -1 && operand->type == OPERAND_IMMEDIATE)
	return false;
    switch(size){
    case 8:
	return true;
    case 16:
	return false;
    case 32:
	if ((type == IS_DISPLACEMENT && ((operand->displacement & 0xfffff000) == 0xfffff000)) ||
	    (type == IS_IMMEDIATE && ((operand->operand & 0xfffff000) == 0xfffff000)))
	    return true;
	return false;
    }
    return true;
}

void Disasm::addRegister(Operand *operand)
{
    if (operand->operand_size == SIZE_BYTE)
	strcat(assemblyCode, Register8[operand->operand]);
    else if (operand->operand_size == SIZE_WORD)
	strcat(assemblyCode, Register16[operand->operand]);
    else if (operand->operand_size == SIZE_FIXED_DWORD)
	strcat(assemblyCode, Register32[operand->operand]);
    else
	strcat(assemblyCode, Register32[operand->operand]);
}

void Disasm::addRetCode()
{
    for (int i = 0; i < currentByte; i++){
	strcat(ret_machineCode, int2str(&machineCode[i], sizeof(INT8), 1, 0, false));
	strcat(ret_machineCode, " ");
    }
}

void Disasm::addImmediate(char *str, Operand *operand, INT8 size, bool noRegister, INT8 type, bool fixedSize, bool isAbsolute)
{
    int isExtend = (type == IS_IMMEDIATE && instruction->s == 1), s = 0, i = 0;
    if (size == SIZE_BYTE)
	s = 8;
    else if (size == SIZE_WORD || size == ABSOLUTE_ADDRESS_FAR_WORD)
	s = 16;
    else if (size == SIZE_DWORD || size == ABSOLUTE_ADDRESS_FAR_DWORD)
	s = 32;

    INT32 immediate = -1;
    if (type == IS_IMMEDIATE || type == IS_OFFSET)
	immediate = operand->operand;
    else if (type == IS_DISPLACEMENT)
	immediate = operand->displacement;
    else if (type == IS_NEW_EIP)
	immediate = instruction->new_eip;
    else if (type == IS_NEW_CS)
	immediate = instruction->new_cs;

    bool minus = false;
    if ((s == 8 && immediate & 0x00000080) || (s == 16 && immediate & 0x00008000) || (s == 32 && immediate & 0x80000000))
	minus = true;

    if (minus){
	if (type == IS_IMMEDIATE){
	    if (!isAbsolute && needToChangeSign(operand, s, IS_IMMEDIATE)){
		strcat(str, "-");
		immediate *= -1;
	    }
	}
	else{
	    if (!noRegister){
		if (needToChangeSign(operand, s, IS_DISPLACEMENT)){
		    strcat(str, " - ");
		    immediate *= -1;
		}
		else{
		    strcat(str, " + ");
		}
	    }
	}

	if (isExtend && (type == IS_IMMEDIATE))
	    strcat(str, int2str(&immediate, s / 8, fixedSize, 1));
	else
	    strcat(str, int2str(&immediate, s / 8, fixedSize, 0));
    }
    else if (immediate == 0){
	if (noRegister){
	    if (!fixedSize)
		strcat(str, "0");
	    else{
		for (; i < s / 4; i++)
		    strcat(str, "0");
	    }
	}
    }
    else{
	if (!noRegister)
	    strcat(str, " + ");
	strcat(str, int2str(&immediate, s / 8, fixedSize, 0));
    }
}

void Disasm::copyOperand(Operand *operand, INT8 operand_number)
{
    char num[10];
    static int operand_order = FIRST_OPERAND;
    if (operand_number == FIRST_OPERAND)
	operand_order = FIRST_OPERAND;
    if (operand == NULL)
	return;
    if (operand_order != FIRST_OPERAND)
	strcat(assemblyCode, ", ");
    operand_order++;

    if (operand->type == OPERAND_REGISTER)
	addRegister(operand);
    else if (operand->type == OPERAND_MEMORY || operand->type == OPERAND_MEMORY_OFFSET ||
	     operand->type == OPERAND_SSE_MEMORY || operand->type == OPERAND_MMX_MEMORY ||
	     operand->type == OPERAND_MEMORY_MEMORY){
	if (operand->operand_size == SIZE_BYTE)
	    strcat(assemblyCode, "byte ptr ");
	else if (operand->operand_size == SIZE_WORD)
	    strcat(assemblyCode, "word ptr ");
	else if (operand->operand_size == SIZE_DWORD)
	    strcat(assemblyCode, "dword ptr ");
	else if (operand->operand_size == SIZE_FIXED_DWORD)
	    strcat(assemblyCode, "dword ptr ");
	else if (operand->operand_size == SIZE_FWORD)
	    strcat(assemblyCode, "fword ptr ");
	else if (operand->operand_size == SIZE_FIXED_FWORD)
	    strcat(assemblyCode, "fword ptr ");
	else if (operand->operand_size == SIZE_QWORD)
	    strcat(assemblyCode, "qword ptr ");
	else if (operand->operand_size == SIZE_FIXED_QWORD)
	    strcat(assemblyCode, "qword ptr ");
	else if (operand->operand_size == SIZE_TBYTE)
	    strcat(assemblyCode, "tbyte ptr ");
	else if (operand->operand_size == SIZE_DQWORD)
	    strcat(assemblyCode, "dqword ptr ");
	else{
	    switch(operand->operand_size){
	    case SIZE_512BYTE:
		strcpy(num, "512");
		break;
	    case SIZE_108BYTE:
		strcpy(num, "108");
		break;
	    case SIZE_94BYTE:
		strcpy(num, "94");
		break;
	    case SIZE_28BYTE:
		strcpy(num, "28");
		break;
	    case SIZE_14BYTE:
		strcpy(num, "14");
		break;
	    default:
		strcpy(num, "\?\?\?");
		break;
	    }

	    strcat(assemblyCode, "<");
	    strcat(assemblyCode, num);
	    strcat(assemblyCode, "-byte");
	    strcat(assemblyCode, "> ptr ");
	}

	strcat(assemblyCode, SegmentRegisters[operand->segment & 7]);
	strcat(assemblyCode, ":[");

	if (operand->type == OPERAND_MEMORY_OFFSET)
	    addImmediate(assemblyCode, operand, operand->addressing_size, HAS_NO_REGISTER, IS_OFFSET, IS_NOT_FIXED_SIZE, IS_NOT_ABSOLUTE);
	else{
	    if (operand->operand != -1){
		if (operand->addressing_size == SIZE_WORD)
		    strcat(assemblyCode, Register16_ModRM[operand->operand & 0xf]);
		else
		    strcat(assemblyCode, Register32[operand->operand & 0xf]);
	    }
	    if (operand->base != -1 && operand->base != 0x5)
		strcat(assemblyCode, Register32[operand->base & 7]);
	    if (operand->index != -1 && operand->index != 0x4){
		if (operand->base != -1 && operand->base != 0x5)
		    strcat(assemblyCode, " + ");
		strcat(assemblyCode, Register32[operand->index & 7]);
		if (operand->scale != -1 && operand->scale != 0){
		    strcat(assemblyCode, " * ");
		    sprintf(num, "%d", (int)pow(2.0, operand->scale));
		    strcat(assemblyCode, num);
		}
	    }
	    if (operand->displacement_size != -1){
		int noRegister = 0;
		if (operand->operand == -1 && (operand->base == -1 || operand->base == 0x5)
		    && (operand->index == -1 || operand->index == 0x4) && operand->scale == -1)
		    noRegister = 1;

		addImmediate(assemblyCode, operand, operand->displacement_size, noRegister, IS_DISPLACEMENT, IS_NOT_FIXED_SIZE, IS_NOT_ABSOLUTE);
	    }
	}
	strcat(assemblyCode, "]");
    }
    else if (operand->type == OPERAND_REGISTER || operand->type == OPERAND_SSE_REGISTER ||
	     operand->type == OPERAND_MMX_REGISTER){
	if (operand->operand == -1)
	    operand->operand = instruction->regop & 7;

	if (operand->type == OPERAND_REGISTER)
	    addRegister(operand);
	else{
	    if (operand->operand_size == SIZE_XMM)
		strcat(assemblyCode, XMMRegister[operand->operand]);
	    else
		strcat(assemblyCode, MMRegister[operand->operand]);
	}
    }
    else if (operand->type == OPERAND_IMMEDIATE)
	addImmediate(assemblyCode, operand, operand->operand_size, HAS_NO_REGISTER, IS_IMMEDIATE, IS_NOT_FIXED_SIZE, IS_NOT_ABSOLUTE);
    else if (operand->type == OPERAND_SEGMENT)
	strcat(assemblyCode, SegmentRegisters[operand->operand]);
    else if (operand->type == OPERAND_FLOW){
	char temp_cs[10], temp_eip[10];
	strcpy(temp_cs, "\0");
	strcpy(temp_eip, "\0");
	if (operand->operand_size == RELATIVE_ADDRESS_SHORT){
	    addImmediate(temp_eip, operand, SIZE_DWORD, HAS_NO_REGISTER, IS_NEW_EIP, IS_FIXED_SIZE, IS_NOT_ABSOLUTE);
	    strcat(assemblyCode, temp_eip);
	}
	else if (operand->operand_size == ABSOLUTE_ADDRESS_FAR_DWORD || operand->operand_size == ABSOLUTE_ADDRESS_FAR_WORD){
	    addImmediate(temp_eip, operand, operand->operand_size, HAS_NO_REGISTER, IS_NEW_EIP, IS_FIXED_SIZE, IS_ABSOLUTE);
	    addImmediate(temp_cs, operand, SIZE_WORD, HAS_NO_REGISTER, IS_NEW_CS, IS_FIXED_SIZE, IS_ABSOLUTE);

	    strcat(assemblyCode, temp_cs);
	    strcat(assemblyCode, ":");
	    strcat(assemblyCode, temp_eip);
	}
	else if (operand->operand_size == RELATIVE_ADDRESS_NEAR_DWORD || operand->operand_size == RELATIVE_ADDRESS_NEAR_WORD){
	    addImmediate(temp_eip, operand, SIZE_DWORD, HAS_NO_REGISTER, IS_NEW_EIP, IS_FIXED_SIZE, IS_NOT_ABSOLUTE);

	    strcat(assemblyCode, temp_eip);
	}
    }
    else if (operand->type == OPERAND_FLOAT)
	strcat(assemblyCode, FloatPointRegister[operand->operand & 7]);
    else if (operand->type == OPERAND_CR)
	strcat(assemblyCode, ControlRegister[operand->operand & 7]);
    else if (operand->type == OPERAND_DR)
	strcat(assemblyCode, DebugRegister[operand->operand & 7]);
}

void Disasm::copyPrefix(bool onlyPrefix){
    if (onlyPrefix){
	memcpy(instruction, &specialInstruction[SPECIAL_PREFIX], sizeof(INSTRUCTION));
	strcpy(assemblyCode, "prefix ");

	switch((unsigned char)machineCode[currentByte - 1]){
	case 0xf0:
	    strcat(assemblyCode, "lock");
	    break;
	case 0xf2:
	    strcat(assemblyCode, "repne");
	    break;
	case 0xf3:
	    strcat(assemblyCode, "rep");
	    break;
	case 0x2e:
	    strcat(assemblyCode, "cs");
	    break;
	case 0x36:
	    strcat(assemblyCode, "ss");
	    break;
	case 0x3e:
	    strcat(assemblyCode, "ds");
	    break;
	case 0x26:
	    strcat(assemblyCode, "es");
	    break;
	case 0x64:
	    strcat(assemblyCode, "fs");
	    break;
	case 0x65:
	    strcat(assemblyCode, "gs");
	    break;
	case 0x66:
	    strcat(assemblyCode, "datasize");
	    break;
	case 0x67:
	    strcat(assemblyCode, "addrsize");
	    break;
	default:
	    strcat(assemblyCode, "???");
	    break;
	}
	strcat(assemblyCode, ":");
    }
    else{
	if (lockAndRepeat != -1){
	    strcat(assemblyCode, RepeatPrefixes[lockAndRepeat & 7]);
	    strcat(assemblyCode, " ");
	}
    }
}

void Disasm::copyAssembleAndMachineCode(bool onlyPrefix)
{
    copyPrefix(onlyPrefix);
    strcat(assemblyCode, instruction->mnemonic);
    copyOperand(instruction->dest, FIRST_OPERAND);
    copyOperand(instruction->src1, SECOND_OPERAND);
    copyOperand(instruction->src2, THIRD_OPERAND);
    copyOperand(instruction->src3, FOURTH_OPERAND);
    addRetCode();
}

void Disasm::copyInstruction(INSTRUCTION *instr)
{
    instruction->assembly = assemblyCode;
    instruction->ret_machineCode = ret_machineCode;

    memcpy(instr, instruction, sizeof(INSTRUCTION));
    instr->lockAndRepeat = lockAndRepeat;
    instr->segmentOverride = segmentOverride;
    instr->OperandSizeOverride = OperandSizeOverride;
    instr->AddressSizeOverride = AddressSizeOverride;

    instr->address = address;
    //if (instr->new_eip != -1)
	//instr->final_address = instr->new_eip;
    //else
	//instr->final_address = instr->address + currentByte;

    instr->size = currentByte;

    instr->binary = (UINT8*)malloc(currentByte);
    memcpy(instr->binary, machineCode, currentByte);
    free(instruction);
}

void Disasm::setHandlerIndex(){
    if (!instruction->dest){
	instruction->handlerIndex = HANDLER_32;
	return;
    }

    switch(instruction->dest->operand_size){
    case SIZE_BYTE: case RELATIVE_ADDRESS_SHORT:
	instruction->handlerIndex = HANDLER_8;
	break;
    case SIZE_WORD: case ABSOLUTE_ADDRESS_FAR_WORD: case RELATIVE_ADDRESS_NEAR_WORD:
	instruction->handlerIndex = HANDLER_16;
	break;
    default:
	instruction->handlerIndex = HANDLER_32;
	break;
    }
}

INT32 Disasm::disassembler(INT8 MACHINECODE[], int CODESIZE, INT32 addr, INT32 baseAddress, INSTRUCTION *instr)
{
    if (CODESIZE == 0)
	return NOT_ENOUGH_CODE;

    instruction = (INSTRUCTION *)malloc(sizeof(INSTRUCTION));
    assemblyCode = (char *)malloc(100);
    ret_machineCode = (char *)malloc(100);
    strcpy(assemblyCode, "\0");
    strcpy(ret_machineCode, "\0");
    machineCode = MACHINECODE;
    currentByte = 0;
    hasPrefix = 0;
    codeSize = CODESIZE;
    INT8 ret = -1;
    address = addr;
    base = baseAddress;
    waitPrefix = -1;
    lockAndRepeat = -1;
    segmentOverride = -1;
    OperandSizeOverride = -1;
    AddressSizeOverride = -1;

    // prefix section
    while (currentByte < (int)codeSize){
	ret = decodePrefix();
	if (ret == COLLIDED_PREFIX){             // collided prefix
	    hasPrefix = 1;
	    break;
	}
	else if (ret == NOT_PREFIX)          // not prefix
	    break;
	else                                 // continue decoding
	    hasPrefix++;
    }
    if ((codeSize == currentByte || ret == COLLIDED_PREFIX) && waitPrefix == -1){   // only prefix or collide prefix
	if (ret == COLLIDED_PREFIX)
	    currentByte = hasPrefix;
	copyAssembleAndMachineCode(ONLY_PREFIX);
	copyInstruction(instr);
	return hasPrefix;
    }
    else if (codeSize == currentByte && waitPrefix != -1){
	waitPrefix = -1;
	currentByte--;
    }

    // opcode section
    if ((unsigned char)machineCode[currentByte] == 0x0f){
	if (codeSize == currentByte)
	    return NOT_ENOUGH_CODE;
	currentByte++;
	memcpy(instruction, &getTwoByteOpcode[(unsigned char)machineCode[currentByte++]], sizeof(INSTRUCTION));
	instruction->opcode = 0x0f00 + (unsigned char)machineCode[currentByte - 1];
    }
    else{
	memcpy(instruction, &getOneByteOpcode[(unsigned char)machineCode[currentByte++]], sizeof(INSTRUCTION));
	instruction->opcode = (unsigned char)machineCode[currentByte - 1];
    }

    if (instruction->type == NEED_FURTHER_PARSED){
	if (codeSize == currentByte)
	    return NOT_ENOUGH_CODE;
	INT8 auxiliary = (machineCode[currentByte] >> 3) & 7;
	INT32 opcode = instruction->opcode;
	memcpy(instruction, &getOneByteWithAuxiliaryOpcode[instruction->dest->operand][auxiliary & 7], sizeof(INSTRUCTION));
	instruction->opcode = opcode;
    }

    if (instruction->opcode >= 0xd8 && instruction->opcode <= 0xdf)
	preprocessingFloatPointInstruction();
    if (instruction->type == SSE1_INSTRUCTION || instruction->type == SSE2_INSTRUCTION ||
	instruction->type == SSE3_INSTRUCTION || instruction->type == SSSE3_INSTRUCTION ||
	instruction->type == SSE41_INSTRUCTION || instruction->type == SSE42_INSTRUCTION ||
	instruction->type == MMX_INSTRUCTION)
	SSEAndMMXInstructionProcess();
    if (instruction->type == SPECIAL_MNEMONIC){
	if (specialMnemonicProcess() == NOT_ENOUGH_CODE)
	    return NOT_ENOUGH_CODE;
    }

    if (validatePrefix() == PREFIX_NOT_QUALIFY){
	currentByte = 1;
	copyAssembleAndMachineCode(ONLY_PREFIX);
	copyInstruction(instr);
	return 1;
    }

    Operand *temp;
    if (instruction->dest){
	temp = instruction->dest;
	instruction->dest = (Operand*)malloc(sizeof(Operand));
	memcpy(instruction->dest, temp, sizeof(Operand));
    }
    if (instruction->src1){
	temp = instruction->src1;
	instruction->src1 = (Operand*)malloc(sizeof(Operand));
	memcpy(instruction->src1, temp, sizeof(Operand));
    }
    if (instruction->src2){
	temp = instruction->src2;
	instruction->src2 = (Operand*)malloc(sizeof(Operand));
	memcpy(instruction->src2, temp, sizeof(Operand));
    }
    if (instruction->src3){
	temp = instruction->src3;
	instruction->src3 = (Operand*)malloc(sizeof(Operand));
	memcpy(instruction->src3, temp, sizeof(Operand));
    }

    // modr/m, sib, immediate section, if exists
    if (instruction->ModRM && instruction->type != FLOAT_INSTRUCTION){
	if (codeSize == currentByte)
	    return NOT_ENOUGH_CODE;
	instruction->mod = (machineCode[currentByte] >> 6) & 3;
	instruction->rm = (machineCode[currentByte]) & 7;
	instruction->regop = (machineCode[currentByte] >> 3) & 7;
    }

    // decode 3 operands, if exists
    if (decodeModRM(instruction->dest, FIRST_OPERAND) == NOT_ENOUGH_CODE)
	return NOT_ENOUGH_CODE;
    if (decodeModRM(instruction->src1, SECOND_OPERAND) == NOT_ENOUGH_CODE)
	return NOT_ENOUGH_CODE;
    if (decodeModRM(instruction->src2, THIRD_OPERAND) == NOT_ENOUGH_CODE)
	return NOT_ENOUGH_CODE;
    if (decodeModRM(instruction->src3, FOURTH_OPERAND) == NOT_ENOUGH_CODE)
	return NOT_ENOUGH_CODE;

    setHandlerIndex();

    copyAssembleAndMachineCode(NOT_ONLY_PREFIX);
    copyInstruction(instr);

    // if (instr->type == FLOW_INSTRUCTION){
    //	instr->printInstrDetail();
    //	exit(0);
    // }

    return currentByte;
}

void Disasm::disassembleMachineCode(char *m, char *b, bool print){
    if (m == NULL || strlen(m) == 0 || !isHex(m)){
	printf("Error: No valid input provided\n");
	return;
    }

    char *hex_machineCode = string2hex(m);

    int size = MAX_INSTRUCTION_SIZE, start = 0, code_size = strlen(m) / 2, sizeForBase;
    INT8 buffer[MAX_INSTRUCTION_SIZE + 1];
    INT8 ret = -1;
    INSTRUCTION instr;
    //FILE *output = fopen("for_testdis", "w+"); // for test
    char result[200]; // for test
    UINT32 address = (b == NULL ? 0x00400000 : string2bin(b, sizeForBase));

    while (1){
	if (start + (int)MAX_INSTRUCTION_SIZE > code_size){
	    size = code_size - start;
	    //if ((INT32)MAX_INSTRUCTION_SIZE > code_size)
	    //size -= 1;
	}
	if (size <= 0 || start >= code_size)
	    break;
	memcpy(buffer, hex_machineCode + start, size);

	ret = disassembler(buffer, size, address, 0x00400000, &instr);
	if (ret == NOT_ENOUGH_CODE){
	    printf("%s\n", m);
	    printf("Not enough code.\n");
	    break;
	}

	if (print)
	    printf("%-10s %-20s %s\n", int2str(&address, sizeof(UINT32), 1, 0), instr.ret_machineCode, instr.assembly);
	//sprintf(result, "%s %s %s\n", int2str(&address, sizeof(UINT32), 1, 0), instr.ret_machineCode, instr.assembly);
	sprintf(result, "%s\n", instr.assembly);
	//fwrite(&result, strlen(result), 1, output);

	address += ret;
	start += ret;
    }
    //fclose(output);
}

void Disasm::disassemblePlainFile(char *p, char *b){
    FILE *plainFile = fopen(p, "r");
    if (plainFile == NULL)
	displayAndExit("Not a valid plain file");
    FILE *to_file = fopen("for_comparison.txt", "w+");
    FILE *to_test = fopen("goodTest.txt", "w+");
    char hehe[MAX_INSTRUCTION_SIZE * 2 + 1], result[200];
    INT8 *buffer;
    int start = 0;
    INT8 ret = -1;
    int address = 0x00400000, size = MAX_INSTRUCTION_SIZE * 2, fileSize;
    struct stat buf;
    INSTRUCTION instr;

    fstat(fileno(plainFile), &buf);
    fileSize = buf.st_size;

    while (1){
	if (start >= fileSize)
	    break;
	if (start + (int)MAX_INSTRUCTION_SIZE * 2 > fileSize)
	    size = fileSize - start;

	fseek(plainFile, start, SEEK_SET);
	fread(&hehe, size, 1, plainFile);
	hehe[size] = '\0';

	if (feof(plainFile) || !isHex(hehe))
	    break;

	buffer = string2hex(hehe);

	ret = disassembler(buffer, size / 2, address, 0x00400000, &instr);
	if (ret == NOT_ENOUGH_CODE)
	    break;

	sprintf(result, "%s %s %s\n", int2str(&instr.address, sizeof(INT32), 1, 0, false), instr.ret_machineCode, instr.assembly);
	//sprintf(result, "%s %s\n", instr.ret_machineCode, instr.assembly);
	fwrite(&result, strlen(result), 1, to_file);
	if (strchr(instr.assembly, '?') == NULL && strstr(instr.assembly, "prefix") == NULL){
	    sprintf(result, "%s\n", instr.assembly);
	    fwrite(&result, strlen(result), 1, to_test);
	}

	address += ret;
	start += ret * 2;
    }
    fclose(to_file);
    fclose(to_test);
}

void Disasm::testDisasm(){
    INT8 hehe[MAX_INSTRUCTION_SIZE + 1];
    char result[200];
    INT32 address = 0x00400000, ret = -1;
    int start = 0;
    FILE *test_file = fopen("hehe", "rb");
    if (test_file == NULL)
	displayAndExit("Binary file not exist or not name `hehe`");
    FILE *to_file = fopen("for_comparison.txt", "w+");
    FILE *to_test = fopen("goodTest.txt", "w+");
    INT32 size = MAX_INSTRUCTION_SIZE;
    INSTRUCTION *instr = (INSTRUCTION *)malloc(sizeof(INSTRUCTION));

    while (1){
	if (start + MAX_INSTRUCTION_SIZE > 0x45fc)
	    size = 0x45fc - start + 1;
	fseek(test_file, start, SEEK_SET);
	fread(&hehe, size, 1, test_file);

	if (feof(test_file) || ferror(test_file))
	    break;

	ret = disassembler(hehe, size, address, 0x00400000, instr);
	if (ret == NOT_ENOUGH_CODE)
	    break;

	sprintf(result, "%s %s %s\n", int2str(&instr->address, sizeof(INT32), 1, 0, false), instr->ret_machineCode, instr->assembly);
	fwrite(&result, strlen(result), 1, to_file);
	if (strchr(instr->assembly, '?') == NULL && strstr(instr->assembly, "prefix") == NULL){
	    sprintf(result, "%s\n", instr->assembly);
	    fwrite(&result, strlen(result), 1, to_test);
	}

	address += ret;
	start += ret;
    }
    free(instr);
    fclose(to_file);
    fclose(to_test);
}
