#include "operand.h"

#include <fstream>
#include <set>
using namespace std;

Operand::Operand(): segment(0),operand(0),
		    operand_size(SIZE_UNDEFINED),type(0),scale(0),
		    index(0),base(0),displacement(0),
		    displacement_size(0),addressing_size(SIZE_UNDEFINED),
		    isDefault(false)
{ }

Operand::Operand(INT8 seg, INT32 oper, OPERAND_SIZE oper_size,
		 OPERAND_TYPE type, INT8 scale, INT8 idx, INT8 base,
		 INT32 disp, int disp_size, OPERAND_SIZE addr_size,
		 bool isDef):
    segment(seg), operand(oper),
    operand_size(oper_size), type(type),
    scale(scale), index(idx), base(base),
    displacement(disp), displacement_size(disp_size),
    addressing_size(addr_size), isDefault(isDef)
{ }

Operand::Operand(INT8 seg, INT32 oper, OPERAND_SIZE oper_size,
		 OPERAND_TYPE type, INT8 scale, INT8 idx, INT8 base,
		 INT32 disp, int disp_size, OPERAND_SIZE addr_size,
		 int isDef):
    segment(seg), operand(oper),
    operand_size(oper_size), type(type),
    scale(scale), index(idx), base(base),
    displacement(disp), displacement_size(disp_size),
    addressing_size(addr_size), isDefault(isDef)
{ }

Operand::Operand(int seg, int oper, OPERAND_SIZE oper_size,
		 OPERAND_TYPE type, int scale, int idx, int base,
		 int disp, int disp_size, OPERAND_SIZE addr_size,
		 int isDef)
{
    this->segment = (INT8)seg;
    this->operand = (INT32)oper;
    this->operand_size = oper_size;
    this->type = type;
    this->scale = (INT8)scale;
    this->index = (INT8)idx;
    this->base = (INT8)base;
    this->displacement = (INT32)disp;
    this->displacement_size = disp_size;
    this->addressing_size = addr_size;
    this->isDefault = isDef;
}

Operand::Operand(int seg, HAVE_AUXILIARY_CODE oper, OPERAND_SIZE oper_size,
		 OPERAND_TYPE type, int scale, int idx, int base,
		 int disp, int disp_size, OPERAND_SIZE addr_size,
		 int isDef)
{
    this->segment = (INT8)seg;
    this->operand = (INT32)oper;
    this->operand_size = oper_size;
    this->type = type;
    this->scale = (INT8)scale;
    this->index = (INT8)idx;
    this->base = (INT8)base;
    this->displacement = (INT32)disp;
    this->displacement_size = disp_size;
    this->addressing_size = addr_size;
    this->isDefault = isDef;
}

INT32 Operand::getOperand() {
    return this->operand;
}

void Operand::setOperand(INT32 displacement) {
    operand = displacement;
}

bool Operand::addr_above_ebp() {
    bool res = false;
    //if (operand == EBP || base == EBP || index == EBP)
    int temp = (INT8)(displacement);
    if (operand == EBP && temp > 0)
        res = true;
    return res;
}

void Operand::accumulate_reg(set<int> &used) 
{
    if (operand != 0xffffffff)
        used.insert(operand);
    else {
        used.insert(index);
        used.insert(base);
    }
}

// change from 8(%ebp) to 8(%ebp, %eax, 1)
void Operand::changed_to_sib(int reg)
{
    base = operand;
    index = reg;
    scale = 0;
    operand = 0xffffffff;
}

void Operand::printOperandDetail(){
    cout << "Type: ";
    switch (this->type){
    case OPERAND_IMMEDIATE:
	cout << "OPERAND_IMMEDIATE" << endl;
	break;
    case OPERAND_MEMORY:
	cout << "OPERAND_MEMORY" << endl;
	break;
    case OPERAND_REGISTER:
	cout << "OPERAND_REGISTER" << endl;
	break;
    case OPERAND_SEGMENT:
	cout << "OPERAND_SEGMENT" << endl;
	break;
    case OPERAND_FLOAT:
	cout << "OPERAND_FLOAT" << endl;
	break;
    case OPERAND_FLOW:
	cout << "OPERAND_FLOW" << endl;
	break;
    case OPERAND_MEMORY_OFFSET:
	cout << "OPERAND_MEMORY_OFFSET" << endl;
	break;
    case OPERAND_LDTR:
	cout << "OPERAND_LDTR" << endl;
	break;
    case OPERAND_TR:
	cout << "OPERAND_TR" << endl;
	break;
    case OPERAND_GDTR:
	cout << "OPERAND_GDTR" << endl;
	break;
    case OPERAND_IDTR:
	cout << "OPERAND_IDTR" << endl;
	break;
    case OPERAND_MSW:
	cout << "OPERAND_MSW" << endl;
	break;
    case OPERAND_CR:
	cout << "OPERAND_CR" << endl;
	break;
    case OPERAND_DR:
	cout << "OPERAND_DR" << endl;
	break;
    case OPERAND_SSE_MEMORY:
	cout << "OPERAND_SSE_MEMORY" << endl;
	break;
    case OPERAND_SSE_REGISTER:
	cout << "OPERAND_SSE_REGISTER" << endl;
	break;
    case OPERAND_MSR:
	cout << "OPERAND_MSR" << endl;
	break;
    case OPERAND_PMC:
	cout << "OPERAND_PMC" << endl;
	break;
    case OPERAND_MMX_MEMORY:
	cout << "OPERAND_MMX_MEMORY" << endl;
	break;
    case OPERAND_MMX_REGISTER:
	cout << "OPERAND_MMX_REGISTER" << endl;
	break;
    case OPERAND_UNDEFINED:
	cout << "OPERAND_UNDEFINED" << endl;
	break;
    case OPERAND_LABEL:
	cout << "OPERAND_LABEL" << endl;
	break;
    case OPERAND_MEMORY_REGISTER:
	cout << "OPERAND_MEMORY_REGISTER" << endl;
	break;
    case OPERAND_MEMORY_MEMORY:
	cout << "OPERAND_MEMORY_MEMORY" << endl;
	break;
    case OPERAND_SSEMEM:
	cout << "OPERAND_SSEMEM" << endl;
	break;
    case OPERAND_MEMORY_SSE:
	cout << "OPERAND_MEMORY_SSE" << endl;
	break;
    case OPERAND_MMXMEM:
	cout << "OPERAND_MMXMEM" << endl;
	break;
    case OPERAND_MEMORY_MMX:
	cout << "OPERAND_MEMORY_MMX" << endl;
	break;
    case OPERAND_MEMORY_FLOAT:
	cout << "OPERAND_MEMORY_FLOAT" << endl;
	break;
    case OPERAND_FLOATMEM:
	cout << "OPERAND_FLOATMEM" << endl;
	break;
    case OPERAND_FLOWIMM:
	cout << "OPERAND_FLOWIMM" << endl;
	break;
    default:
	cout << "UNKNOWN OPERAND TYPE" << endl;
	break;
    }

    cout << "Operand: " << int2str((void*)&this->operand, sizeof(UINT32), 1, 0, false) << endl;

    cout << "Operand size: " << this->operand_size << endl;

    cout << "Scale: " << int2str((void*)&this->scale, sizeof(UINT8), 1, 0, false) << endl;
    cout << "Index: " << int2str((void*)&this->index, sizeof(UINT8), 1, 0, false) << endl;
    cout << "Base: " << int2str((void*)&this->base, sizeof(UINT8), 1, 0, false) << endl;

    cout << "Segment: ";
    switch ((UINT8)this->segment){
    case ES:
	cout << "ES" << endl;
	break;
    case CS:
	cout << "ES" << endl;
	break;
    case SS:
	cout << "ES" << endl;
	break;
    case DS:
	cout << "ES" << endl;
	break;
    case FS:
	cout << "ES" << endl;
	break;
    case GS:
	cout << "ES" << endl;
	break;
    default:
	cout << (INT32)segment << endl;
	break;
    }

    cout << "Displacement: " << int2str((void*)&this->displacement, sizeof(UINT32), 1, 0, false) << endl;
    cout << "Displacement size: " << this->displacement_size << endl;

    cout << "Addressing size: " << this->addressing_size << endl;
}

ostream& operator<<(ostream &os, Operand o)
{
    os << o.type << "\t";
    os << o.operand << "\t";
    os << endl;
    return os;
}
