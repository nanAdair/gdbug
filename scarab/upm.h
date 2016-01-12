/*
 * =====================================================================================
 *
 *       Filename:  upm.h
 *
 *    Description:  universal patch mechanism
 *
 *        Version:  1.0
 *        Created:  12/21/2015 15:45:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef GDBUG_SCARAB_UPM_H
#define GDBUG_SCARAB_UPM_H

#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

#include <iostream>

#include "type.h"

typedef enum {
    UPM_INSTR_MEMBER_TYPE,
    UPM_SECTION_MEMBER_TYPE,
    UPM_OFFSET_MEMBER_TYPE,
    UPM_SYMBOL_MEMBER_TYPE,
    UPM_OPERAND_MEMBER_TYPE,
    UPM_FUNCTION_MEMBER_TYPE
} UPM_TYPE;

class SCInstr;
class Section;
class SectionVec;
class RelocationVec;

class Patch 
{
public:
    virtual bool apply() = 0;
    virtual ~Patch() {}
};

class PatchInstrtoInstr : public Patch 
{
public:
    PatchInstrtoInstr(shared_ptr<SCInstr> src, UINT32 s_offset, shared_ptr<SCInstr> dest, UINT32 d_offset, INT32 add) :
        src_instr(src), src_offset(s_offset),
        dest_instr(dest), dest_offset(d_offset),
        addend(add) {}

protected:
    shared_ptr<SCInstr> src_instr;
    UINT32 src_offset;
    shared_ptr<SCInstr> dest_instr;
    UINT32 dest_offset;
    INT32 addend;
};

class PatchInstrtoInstr32 : public PatchInstrtoInstr 
{
public:
    PatchInstrtoInstr32(shared_ptr<SCInstr> src, UINT32 s_offset, shared_ptr<SCInstr> dest, UINT32 d_offset, INT32 add) :
        PatchInstrtoInstr(src, s_offset, dest, d_offset, add) {}
    bool apply();
};

class PatchInstrtoInstrPC32 : public PatchInstrtoInstr 
{
public:
    PatchInstrtoInstrPC32(shared_ptr<SCInstr> src, UINT32 s_offset, shared_ptr<SCInstr> dest, UINT32 d_offset, INT32 add) :
        PatchInstrtoInstr(src, s_offset, dest, d_offset, add) {}
    bool apply();
};

class PatchSectiontoInstr : public Patch
{
public:
    PatchSectiontoInstr(shared_ptr<Section> src, INT32 s_offset, shared_ptr<SCInstr> dest, UINT32 d_offset, INT32 add) :
        src_sec(src), src_offset(s_offset),
        dest_instr(dest), dest_offset(d_offset),
        addend(add) {}
protected:
    shared_ptr<Section> src_sec;
    INT32 src_offset;
    shared_ptr<SCInstr> dest_instr;
    UINT32 dest_offset;
    INT32 addend;
};

class PatchSectiontoInstr32 : public PatchSectiontoInstr
{
public:
    PatchSectiontoInstr32(shared_ptr<Section> src, INT32 s_offset, shared_ptr<SCInstr> dest, UINT32 d_offset, INT32 add):
        PatchSectiontoInstr(src, s_offset, dest, d_offset, add) {}
    bool apply();
};

class PatchSectiontoInstrPC32 : public PatchSectiontoInstr
{
public:
    PatchSectiontoInstrPC32(shared_ptr<Section> src, INT32 s_offset, shared_ptr<SCInstr> dest, UINT32 d_offset, INT32 add):
        PatchSectiontoInstr(src, s_offset, dest, d_offset, add) {}
    bool apply();
};

class PatchSectiontoSection : public Patch
{
public:
    PatchSectiontoSection(shared_ptr<Section> src, UINT32 s_offset, shared_ptr<Section> dest, UINT32 d_offset, INT32 add) :
        src_sec(src), src_offset(s_offset),
        dest_sec(dest), dest_offset(d_offset),
        addend(add) {}
protected:
    shared_ptr<Section> src_sec;
    UINT32 src_offset;
    shared_ptr<Section> dest_sec;
    UINT32 dest_offset;
    INT32 addend;
};

class PatchSectiontoSection32 : public PatchSectiontoSection 
{
public:
    PatchSectiontoSection32(shared_ptr<Section> src, UINT32 s_offset, shared_ptr<Section> dest, UINT32 d_offset, INT32 add):
        PatchSectiontoSection(src, s_offset, dest, d_offset, add) {}
    bool apply();
};

class PatchSectiontoSectionPC32 : public PatchSectiontoSection 
{
public:
    PatchSectiontoSectionPC32(shared_ptr<Section> src, UINT32 s_offset, shared_ptr<Section> dest, UINT32 d_offset, INT32 add):
        PatchSectiontoSection(src, s_offset, dest, d_offset, add) {}
    bool apply();
};

class PatchVec
{
public:
    void add_patch(shared_ptr<Patch> &p)
    { upm_vec.push_back(p); }
    int apply();
private:
    vector<shared_ptr<Patch> > upm_vec;
};
#endif
