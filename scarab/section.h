/*
 * =====================================================================================
 *
 *       Filename:  section.h
 *
 *    Description:  classes related to sections of elf file
 *
 *        Version:  1.0
 *        Created:  11/19/2015 10:41:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GDBUG_SCARAB_SECTION_H
#define GDBUG_SCARAB_SECTION_H

#include <elf.h>

#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <string>
using std::string;
#include <iostream>
using std::ostream;

#include "type.h"

// Sections to be added
#define INTERP_SECTION_NAME     ".interp"
#define HASH_SECTION_NAME       ".hash"
#define DYNSYM_SECTION_NAME     ".dynsym"
#define DYNSTR_SECTION_NAME     ".dynstr"
#define GV_SECTION_NAME         ".gnu.version"
#define GNR_SECTION_NAME        ".gnu.version_r"
#define REL_DYN_SECTION_NAME    ".rel.dyn"
#define REL_PLT_SECTION_NAME    ".rel.plt"
#define PLT_SECTION_NAME        ".plt"
#define DYNAMIC_SECTION_NAME    ".dynamic"
#define GOT_SECTION_NAME        ".got"
#define GOT_PLT_SECTION_NAME    ".got.plt"

#define TEXT_SECTION_NAME       ".text"
#define INIT_SECTION_NAME       ".init"
#define FINI_SECTION_NAME       ".fini"
#define INIT_ARRAY_SECTION_NAME ".init_array"
#define FINI_ARRAY_SECTION_NAME ".fini_array"

#define BSS_SECTION_NAME        ".bss"
#define RODATA_SECION_NAME      ".rodata"
#define EH_FRAME_SECTION_NAME   ".eh_frame"
#define JCR_SECTION_NAME        ".jcr"
#define DATA_SECTION_NAME       ".data"
#define COMMENT_SECTION_NAME    ".comment"
#define SYMTAB_SECTION_NAME     ".symtab"
#define STRTAB_SECTION_NAME     ".strtab"

#define SHSTRTAB_SECTION_NAME   ".shstrtab"
#define NOTE_SECTION_NAME       ".note.ABI-tag"

#define DYNAMIC_NUMBER 28
#define DYNAMIC_ENTSIZE 8

class File;
class SymbolDynVec;

class Section: public std::enable_shared_from_this<Section>
{
public:
    friend ostream &operator<<(ostream &, shared_ptr<Section>&);
    friend ostream &operator<<(ostream &, Section&);
    friend class SectionVec;
    Section() {}
    Section(Elf32_Shdr*, UINT16);
    Section(Elf32_Shdr*, UINT16, const string&);
    Section(Elf32_Shdr*, UINT16, UINT8*, UINT8*);
    virtual ~Section();

    UINT32 get_section_size() const 
    { return size_; }

    UINT32 get_section_addralign() const 
    { return addralign_; }

    UINT32 get_section_delta() const 
    { return delta_; }
    
    UINT16 get_section_index() const
    { return final_index_ == 0 ? origin_index_ : final_index_; }

    shared_ptr<Section> get_merged_section() const 
    { return merge_to_; }

    string get_section_name() const 
    { return name_; }

    UINT8 *get_section_data() const 
    { return data_; }

    void set_section_size(UINT32 s)
    { size_ = s; }

    void set_section_addralign(UINT32 a)
    { addralign_ = a; }

    void expand_section_data(const UINT8 *, UINT32, bool);

protected:
    UINT32 name_offset_;
    UINT32 type_;
    UINT32 flags_;
    UINT32 address_;
    UINT32 file_offset_;
    UINT32 size_;
    UINT32 link_;
    UINT32 info_;
    UINT32 addralign_;
    UINT32 entsize_;
    UINT16 origin_index_;
    UINT16 final_index_;
    UINT32 delta_;
    UINT32 misc_;
    shared_ptr<Section> merge_to_;
    string name_;
    UINT8 *data_;

private:
    void _merge_section(shared_ptr<Section>&);

    Section(const Section&);
    Section &operator=(const Section&);
};

class SectionInterp : public Section 
{
public:
    SectionInterp(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionHash : public Section 
{
public:
    SectionHash(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionDynsym : public Section 
{
public:
    SectionDynsym(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionDynstr : public Section 
{
public:
    SectionDynstr(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
    void set_dynstr_data(const SymbolDynVec&, const vector<string>&);
};

class SectionGnuVersion : public Section 
{
public:
    SectionGnuVersion(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
    SectionGnuVersion(Elf32_Shdr* cur_sec_dr, UINT16 index, UINT8* file_data, UINT8* strn_table):
        Section(cur_sec_dr, index, file_data, strn_table) {}
    UINT16 get_version_number(int offset) const 
    { return data_[entsize_ * offset]; }
};

class SectionGnuVersionR : public Section 
{
public:
    SectionGnuVersionR(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionRelDyn : public Section 
{
public:
    SectionRelDyn(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionRelPlt : public Section 
{
public:
    SectionRelPlt(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionPlt : public Section 
{
public:
    SectionPlt(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionDynamic : public Section
{
public:
    SectionDynamic(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
    SectionDynamic(Elf32_Shdr* cur_sec_dr, UINT16 index, UINT8* file_data, UINT8* strn_table):
        Section(cur_sec_dr, index, file_data, strn_table) {}
    int get_dynamic_attribute(int) const;
};

class SectionGot : public Section 
{
public:
    SectionGot(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionGotPlt : public Section 
{
public:
    SectionGotPlt(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
        Section(cur_sec_dr, index, name) {}
};

class SectionVec
{
public:
    friend ostream &operator<<(ostream &, const SectionVec&);
    SectionVec() {}

    void init(const File&);
    SectionVec merge_sections();
    void fill_sections_content(const string&, const vector<string>&, const SymbolDynVec&);

    shared_ptr<Section> get_section_by_name(const string&) const;
    shared_ptr<Section> get_section_by_index(UINT16) const;

    //shared_ptr<const Section> get_section_by_name(const string& s) const 
    //{ return const_cast<SectionVec*>(this)->get_section_by_name(s); }
    //shared_ptr<const Section> get_section_by_index(UINT16 index) const 
    //{ return const_cast<SectionVec*>(this)->get_section_by_index(index); }

private:
    vector<shared_ptr<Section> > sec_vec_;

    bool _skip_Xsec(const string&) const;

    void _create_sections();
};
#endif
