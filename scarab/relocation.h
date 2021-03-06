/*
 * =====================================================================================
 *
 *       Filename:  relocation.h
 *
 *    Description:  classes related to relocations of elf file
 *
 *        Version:  1.0
 *        Created:  11/20/2015 16:16:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GDBUG_SCARAB_RELOCATION_H
#define GDBUG_SCARAB_RELOCATION_H

#include <elf.h>

#include <vector>
using std::vector;
#include <iostream>
using std::ostream;
#include <memory>
using std::shared_ptr;
using std::make_shared;

#include "type.h"

class FileRel;
class Section;
class SectionVec;
class Symbol;
class SymbolVec;
class SymbolDynVec;
class InstrList;
class PatchVec;

class Relocation
{
public:
    friend ostream &operator<<(ostream &os, const Relocation &rel);
    friend class RelocationVec;
    Relocation() {}
    Relocation(Elf32_Rel*, const FileRel&, const SectionVec&, const SectionVec&, UINT32, const SymbolVec&);

    UINT8 get_relocation_type() const 
    { return type_; }

    void apply_relocation_32();
    void apply_relocation_pc32(const SectionVec&, const SymbolDynVec&);
    void apply_relocation_gotpc();
    void apply_relocation_gotoff(const SectionVec&);
    void apply_relocation_got32(const SectionVec&, const SymbolDynVec&);
    void apply_relocation_plt32(const SectionVec&, const SymbolDynVec&);

private:
    UINT32 offset_;
    UINT8 type_;
    UINT32 value_;
    INT32 addend_;
    shared_ptr<Section> sec_;
    shared_ptr<Symbol> sym_;

    void _markGOTPLTSymbol();
};

class RelocationVec
{
public:
    friend ostream &operator<<(ostream &os, const RelocationVec &r);
    RelocationVec() {}

    void init(const FileRel&, SectionVec&, const SectionVec&, const SymbolVec&);
    void apply_relocations(const SectionVec&, const SymbolDynVec&);
    void construct_upm(const SectionVec&, PatchVec&);
private:
    vector<shared_ptr<Relocation> > rel_vec_;

    RelocationVec(const RelocationVec&);
    RelocationVec &operator=(const RelocationVec&);
};
#endif
