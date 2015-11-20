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
using std::weak_ptr;
using std::make_shared;
#include <string>
using std::string;
#include <iostream>
using std::ostream;

#include "type.h"

class File;

class Section
{
public:
    friend ostream &operator<<(ostream &, shared_ptr<Section>&);
    friend ostream &operator<<(ostream &, Section&);
    friend class SectionVec;
    Section() {}
    Section(Section*);
    Section(Elf32_Shdr*, UINT16, UINT8*, UINT8*);
    ~Section();
    
    UINT16 get_section_index() const
    { return final_index_ == 0 ? origin_index_ : final_index_; }
private:
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
    Section* merge_to_;
    //weak_ptr<Section> merge_to_;
    string name_;
    UINT8 *data_;

    void _merge_section(shared_ptr<Section>&);

    Section(const Section&);
    Section &operator=(const Section&);
};

class SectionVec
{
public:
    friend ostream &operator<<(ostream &, const SectionVec&);
    SectionVec() {}

    void init(const File&);
    vector<shared_ptr<Section> > merge_sections();
    shared_ptr<Section> get_section_by_name(const string&);
    shared_ptr<Section> get_section_by_index(UINT16);

    shared_ptr<const Section> get_section_by_name(const string& s) const 
    { return const_cast<SectionVec*>(this)->get_section_by_name(s); }
    shared_ptr<const Section> get_section_by_index(UINT16 index) const 
    { return const_cast<SectionVec*>(this)->get_section_by_index(index); }

private:
    vector<shared_ptr<Section> > sec_vec_;

    bool _skip_Xsec(const string&);
    SectionVec(const SectionVec&);
    SectionVec &operator=(const SectionVec&);
};
#endif
