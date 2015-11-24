/*
 * =====================================================================================
 *
 *       Filename:  symbol.h
 *
 *    Description:  classes related to symbols of elf file
 *
 *        Version:  1.0
 *        Created:  11/20/2015 14:35:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GDBUG_SCARAB_SYMBOL_H
#define GDBUG_SCARAB_SYMBOL_H

#include <elf.h>

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::ostream;
#include <memory>
using std::shared_ptr;
using std::make_shared;

#include "type.h"

#define INIT_ARRAY_START "__init_array_start"
#define INIT_ARRAY_END "__init_array_end"
#define GOT_SYMBOL_NAME "_GLOBAL_OFFSSET_TABLE_"

#define SYM_LOCAL 1
#define SYM_GOT (1 << 1)
#define SYM_PLT (1 << 2)
#define SYM_OUT (1 << 3)

class Section;
class SectionVec;
class FileRel;
class FileDyn;
class VersionVec;

class Symbol
{
public:
    friend ostream& operator<<(ostream & os, const Symbol &sym);
    friend class SymbolVec;
    friend class SymbolDynVec;
    Symbol() {}
    Symbol(Elf32_Sym*, UINT32);
    Symbol(Elf32_Sym*, UINT32, UINT8*, const SectionVec&, const SectionVec&);

    string get_symbol_name() const 
    { return name_; }

    UINT32 get_symbol_type() const 
    { return type_; }

    UINT32 get_symbol_index() const 
    { return index_; }

    UINT16 get_symbol_shndx() const 
    { return shndx_; }

    UINT32 get_symbol_sd_type() const 
    { return sd_type_; }

    bool get_symbol_handle() const 
    { return handled_; }

    void set_symbol_name_offset(UINT32 offset) 
    { name_offset_ = offset; }

    void set_symbol_type(UINT8 type)
    { type_ = type; }

    void set_symbol_index(UINT32 index)
    { index_ = index; }

    void set_symbol_handle(bool h)
    { handled_ = h; }

    void add_symbol_sd_type(int sd)
    { sd_type_ |= sd; }

    void del_symbol_sd_type(int sd)
    { sd_type_ &= ~sd; }

    Elf32_Sym symbol_to_elfsym() const;
    
protected:
    UINT32 name_offset_;
    UINT32 value_;
    UINT32 size_;
    UINT8 type_;
    UINT8 binding_;
    UINT8 other_;
    UINT16 shndx_;
    UINT32 index_;
    string name_;
    shared_ptr<Section> sec_;
    UINT32 sd_type_;
    bool handled_;

private:
    void _handleCOMMON(const SectionVec&); 
};

class SymbolDyn : public Symbol
{
public:
    friend ostream& operator<<(ostream &os, const SymbolDyn &s);
    friend class SymbolDynVec;
    SymbolDyn() {}
    SymbolDyn(Symbol *sym):
        Symbol(*sym),
        version_(0),
        version_name_(""),
        file_("") {}
    SymbolDyn(Elf32_Sym*, UINT32, UINT8*, const SectionVec&, string, const VersionVec&);

    UINT16 get_dynsym_version() const 
    { return version_; }

    string get_dynsym_version_name() const 
    { return version_name_; }

    string get_dynsym_file() const 
    { return file_; }

    void set_dynsym_version(UINT16 v)
    { version_ = v; }

    void set_dynsym_version_name(string n) 
    { version_name_ = n; }

    void set_dynsym_file(string n)
    { file_ = n; }

private:
    UINT16 version_;
    string version_name_;
    string file_;
};

class SymbolVec
{
public:
    friend ostream& operator<<(ostream &os, const SymbolVec &s);
    friend class SymbolDynVec;
    SymbolVec() {}

    void init(const FileRel&, const SectionVec&, const SectionVec&);
    shared_ptr<Symbol> get_symbol_by_index(UINT32) const;

private:
    vector<shared_ptr<Symbol> > sym_vec_;

    SymbolVec(const SymbolVec&);
    SymbolVec &operator=(const SymbolVec&);
};

class SymbolDynVec
{
public:
    friend ostream& operator<<(ostream &os, const SymbolDynVec &s);
    SymbolDynVec() {}
    SymbolDynVec(const FileDyn&, const SectionVec&, const VersionVec&);

    shared_ptr<SymbolDyn> get_dynsym_by_name(string name) const;
    shared_ptr<SymbolDyn> get_dynsym_by_index(UINT32 index) const;
    UINT32 get_dynsym_vec_size() const;
    void addFromSDVec(const SymbolVec&, const SymbolDynVec&);
    string accumulate_names(UINT32) const;
    shared_ptr<SymbolDyn> get_ith_dynsym(UINT32) const;

private:
    vector<shared_ptr<SymbolDyn> > dynsym_vec_;

    void _markDynSymbol(const SymbolVec&, const SymbolDynVec&);
    SymbolDynVec(const SymbolDynVec&);
    SymbolDynVec &operator=(const SymbolDynVec&);
};

#endif
