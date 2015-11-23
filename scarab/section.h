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

class File;

class Section: public std::enable_shared_from_this<Section>
{
public:
    friend ostream &operator<<(ostream &, shared_ptr<Section>&);
    friend ostream &operator<<(ostream &, Section&);
    friend class SectionVec;
    Section() {}
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

class SectionDynamic : public Section
{
public:
    SectionDynamic(Elf32_Shdr* cur_sec_dr, UINT16 index, UINT8* file_data, UINT8* strn_table):
        Section(cur_sec_dr, index, file_data, strn_table) {}
    int get_dynamic_attribute(int) const;
};

class SectionGnuVersion : public Section 
{
public:
    SectionGnuVersion(Elf32_Shdr* cur_sec_dr, UINT16 index, UINT8* file_data, UINT8* strn_table):
        Section(cur_sec_dr, index, file_data, strn_table) {}
    UINT16 get_version_number(int offset) const 
    { return data_[entsize_ * offset]; }
};


class SectionVec
{
public:
    friend ostream &operator<<(ostream &, const SectionVec&);
    SectionVec() {}

    void init(const File&);
    SectionVec merge_sections();
    shared_ptr<Section> get_section_by_name(const string&) const;
    shared_ptr<Section> get_section_by_index(UINT16) const;

    //shared_ptr<const Section> get_section_by_name(const string& s) const 
    //{ return const_cast<SectionVec*>(this)->get_section_by_name(s); }
    //shared_ptr<const Section> get_section_by_index(UINT16 index) const 
    //{ return const_cast<SectionVec*>(this)->get_section_by_index(index); }

private:
    vector<shared_ptr<Section> > sec_vec_;

    bool _skip_Xsec(const string&);
};
#endif
