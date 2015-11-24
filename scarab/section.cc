/*
 * =====================================================================================
 *
 *       Filename:  section.cc
 *
 *    Description:  implementation of section classes
 *
 *        Version:  1.0
 *        Created:  11/19/2015 11:13:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "section.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <fstream>

#include "sectionGD.h"
#include "file.h"
#include "symbol.h"
#include "log.h"
using namespace std;

Section::Section(Elf32_Shdr *cur_sec_dr, UINT16 index):
    name_offset_(cur_sec_dr->sh_name),
    type_(cur_sec_dr->sh_type),
    flags_(cur_sec_dr->sh_flags),
    address_(cur_sec_dr->sh_addr),
    file_offset_(cur_sec_dr->sh_offset),
    size_(cur_sec_dr->sh_size),
    link_(cur_sec_dr->sh_link),
    info_(cur_sec_dr->sh_info),
    addralign_(cur_sec_dr->sh_addralign),
    entsize_(cur_sec_dr->sh_entsize),
    origin_index_(index),
    final_index_(0),
    delta_(0),
    misc_(index),
    data_(NULL)
{
}

Section::Section(Elf32_Shdr *cur_sec_dr, UINT16 index, const string &name):
    Section(cur_sec_dr, index)
{
    name_ = name;
}

Section::Section(Elf32_Shdr *cur_sec_dr, UINT16 index, UINT8 *file_data, UINT8 *strn_table):
    Section(cur_sec_dr, index)
{
    data_ = new UINT8 [size_];
    name_ = string(reinterpret_cast<char *>(strn_table + name_offset_));
    memcpy(data_, file_data + file_offset_, size_);
}

Section::~Section()
{
    if (data_) {
        delete[] data_;
        data_ = NULL;
    }
}

/* Merge sec to the tail of the calling section without changing sec content */
void Section::_merge_section(shared_ptr<Section> &sec)
{
    UINT32 addition, t_datasize, s_datasize, t_newdatasize;
    t_datasize = size_;
    s_datasize = sec->size_;

    /* TODO: the alignment should depend on the real address and the target section's align, may induce bug here */
    UINT32 s_align = sec->addralign_;
    addition = 0;
    while ((t_datasize+addition) % s_align)
        addition++;
    t_newdatasize = t_datasize + addition + s_datasize;

    UINT8 *new_data = new UINT8 [t_newdatasize];
    memset(new_data, 0, t_newdatasize);
    memcpy(new_data, data_, t_datasize);
    memcpy(new_data+t_datasize+addition, sec->data_, s_datasize);

    std::swap(data_, new_data);
    delete [] new_data;

    size_ = t_newdatasize;
    addralign_ = addralign_ > s_align ? addralign_ : s_align;

    sec->merge_to_ = shared_from_this();
    sec->delta_ = t_datasize + addition;
}

void Section::expand_section_data(const UINT8 *to_add, UINT32 to_add_size, bool update_size)
{
    UINT32 new_datasize = size_ + to_add_size;
    UINT8 *new_data = new UINT8 [new_datasize];
    memset(new_data, 0, new_datasize);
    if (data_)
        memcpy(new_data, data_, size_);
    memcpy(new_data+size_, to_add, to_add_size);

    std::swap(data_, new_data);
    if (new_data)
        delete [] new_data;

    if (update_size)
        size_ = new_datasize;
}

int SectionDynamic::get_dynamic_attribute(int tag) const 
{
    for (int i = 0; i < size_ / entsize_; i++) {
        Elf32_Dyn *cur_dyn;
        cur_dyn = reinterpret_cast<Elf32_Dyn*>(data_ + entsize_*i);
        if (cur_dyn->d_tag == tag)
            return cur_dyn->d_un.d_val;
    }
    report(RL_ONE, "wrong tag for dynamic section");
}

void SectionDynstr::set_dynstr_data(const SymbolDynVec &dsl, const vector<string> &so_files)
{
    UINT8 head[] = {0};
    expand_section_data(head, 1, 1);

    for (int i = 0; i < so_files.size(); i++)
        expand_section_data(reinterpret_cast<const UINT8*>(so_files[i].c_str()), so_files[i].size()+1, 1);

    string dynamic_names = dsl.accumulate_names(size_);
    expand_section_data(reinterpret_cast<const UINT8*>(dynamic_names.c_str()), dynamic_names.size()+1, 1);

    for (int i = 0; i < size_; i++)
        if (data_[i] == ';')
            data_[i] = 0;
}

void SectionVec::init(const File& f)
{
    Elf32_Shdr *cur_sec_dr;
    UINT8 *sec_strn_table, *file_data;

    sec_strn_table = f.get_section_strn_table();
    file_data = f.get_file_data();

    for (int i = 0; i < f.get_section_number(); i++) {
        cur_sec_dr = f.get_section_table() + i;
        string sec_name = string(reinterpret_cast<char *>(sec_strn_table + cur_sec_dr->sh_name));
        shared_ptr<Section> sec;
        if (sec_name == ".dynamic")
            sec = make_shared<SectionDynamic>(cur_sec_dr, i, file_data, sec_strn_table);
        else if (sec_name == ".gnu.version") 
            sec = make_shared<SectionGnuVersion>(cur_sec_dr, i, file_data, sec_strn_table);
        else
            sec = make_shared<Section>(cur_sec_dr, i, file_data, sec_strn_table);
        sec_vec_.push_back(sec);
    }

    report(RL_FOUR, "section vector construction completed");
}

SectionVec SectionVec::merge_sections()
{
    SectionVec res;

    shared_ptr<Section> section_text = get_section_by_name(".text");
    vector<shared_ptr<Section> >::iterator it;

    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->flags_ & SHF_MERGE && (*it)->name_ != ".comment") {
            (*(it-1))->_merge_section(*it);
            res.sec_vec_.push_back(*it);
            sec_vec_.erase(it);
            it--;
        }
        else if ((*it)->flags_ & SHF_EXECINSTR && !_skip_Xsec((*it)->name_)) {
            section_text->_merge_section(*it);
            res.sec_vec_.push_back(*it);
            sec_vec_.erase(it);
            it--;
        }
    }

    report(RL_FOUR, "merging sections completed");
    return res;
}

shared_ptr<Section> SectionVec::get_section_by_name(const string &s) const 
{
    vector<shared_ptr<Section> >::const_iterator it;
    shared_ptr<Section> res;
    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->name_ == s)
            return *it;
    }

    return res;
}

shared_ptr<Section> SectionVec::get_section_by_index(UINT16 index) const
{
    vector<shared_ptr<Section> >::const_iterator it;
    shared_ptr<Section> res;
    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->get_section_index() == index)
            return *it;
    }

    return res;
}

bool SectionVec::_skip_Xsec(const string &s) const
{
    string to_skip[] = {
        ".text",
        ".init",
        ".fini"
    };

    for (int i = 0; i < 3; i++) 
        if (to_skip[i] == s)
            return 1;
    return 0;
}

void SectionVec::_create_sections()
{
    UINT32 index = (sec_vec_.back())->get_section_index() + 1;
    for (int i = 0; i < ADDEDSECTIONNUMBER; i++) {
        const string name = AddedSectionsName[i];
        shared_ptr<Section> cur_sec;
        if (name == INTERP_SECTION_NAME)
            cur_sec = make_shared<SectionInterp>(AddedSectionsInfo+i, index+i, name);
        else if (name == DYNSYM_SECTION_NAME)
            cur_sec = make_shared<SectionDynsym>(AddedSectionsInfo+i, index+i, name);
        else if (name == DYNSTR_SECTION_NAME)
            cur_sec = make_shared<SectionDynstr>(AddedSectionsInfo+i, index+i, name);
        else if (name == HASH_SECTION_NAME)
            cur_sec = make_shared<SectionHash>(AddedSectionsInfo+i, index+i, name);
        else if (name == GV_SECTION_NAME)
            cur_sec = make_shared<SectionGnuVersion>(AddedSectionsInfo+i, index+i, name);
        else if (name == GNR_SECTION_NAME)
            cur_sec = make_shared<SectionGnuVersionR>(AddedSectionsInfo+i, index+i, name);
        else if (name == REL_DYN_SECTION_NAME)
            cur_sec = make_shared<SectionRelDyn>(AddedSectionsInfo+i, index+i, name);
        else if (name == REL_PLT_SECTION_NAME)
            cur_sec = make_shared<SectionRelPlt>(AddedSectionsInfo+i, index+i, name);
        else if (name == PLT_SECTION_NAME)
            cur_sec = make_shared<SectionPlt>(AddedSectionsInfo+i, index+i, name);
        else if (name == DYNAMIC_SECTION_NAME)
            cur_sec = make_shared<SectionDynamic>(AddedSectionsInfo+i, index+i, name);
        else if (name == GOT_SECTION_NAME)
            cur_sec = make_shared<SectionGot>(AddedSectionsInfo+i, index+i, name);
        else if (name == GOT_PLT_SECTION_NAME)
            cur_sec = make_shared<SectionGotPlt>(AddedSectionsInfo+i, index+i, name);

        cur_sec->misc_ = 0;
        sec_vec_.push_back(cur_sec);
    }

    report(RL_FOUR, "complete adding empty sections");
}

void SectionVec::fill_sections_content(const string &ld_file, const vector<string> &so_files, const SymbolDynVec &dsl)
{
    _create_sections();
    shared_ptr<Section> dynstr, init;
    dynstr = get_section_by_name(DYNSTR_SECTION_NAME);
    init = get_section_by_name(INIT_SECTION_NAME);

    std::dynamic_pointer_cast<SectionDynstr>(dynstr)->set_dynstr_data(dsl, so_files);
}

/*-----------------------------------------------------------------------------
 *  helper printer functions below
 *-----------------------------------------------------------------------------*/
ostream& operator<<(ostream &os, const SectionVec &s)
{
    for (int i = 0; i < s.sec_vec_.size(); i++)
        os << *(s.sec_vec_[i]);

    return os;
}

ostream& operator<<(ostream &os, Section &sec)
{
    os << hex;
    os << sec.name_ << " ";
    os << sec.size_ << " " << sec.entsize_ << std::endl;
    string temp("temp");
    std::ofstream of;
    of.open(temp+sec.name_, std::ofstream::binary);
    of.write(reinterpret_cast<const char*>(sec.data_), sec.size_);
    of.close();

    return os;
}
