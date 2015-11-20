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

#include "file.h"
#include "log.h"

Section::Section(Elf32_Shdr *cur_sec_dr, UINT16 index, UINT8 *file_data, UINT8 *strn_table):
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
    data_ = new UINT8 [size_];
    name_ = string(reinterpret_cast<char *>(strn_table + name_offset_));
    memcpy(data_, file_data + file_offset_, size_);
}

Section::Section(Section *s):
    name_offset_(s->name_offset_),
    type_(s->type_),
    flags_(s->flags_),
    address_(s->address_),
    file_offset_(s->file_offset_),
    size_(s->size_),
    link_(s->link_),
    info_(s->info_),
    addralign_(s->addralign_),
    entsize_(s->entsize_),
    origin_index_(s->origin_index_),
    final_index_(s->final_index_),
    delta_(s->delta_),
    misc_(s->misc_),
    name_(s->name_),
    data_(s->data_) 
{
    merge_to_ = s->merge_to_;
    //if (auto spt = s->merge_to_.lock())
        //merge_to_ = spt;
    //else
        //merge_to_ = weak_ptr<Section> ();
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

    sec->merge_to_ = this;
    sec->delta_ = t_datasize + addition;
}

void SectionVec::init(const File& f)
{
    Elf32_Shdr *cur_sec_dr;
    UINT8 *sec_strn_table, *file_data;

    sec_strn_table = f.get_section_strn_table();
    file_data = f.get_file_data();

    for (int i = 0; i < f.get_section_number(); i++) {
        cur_sec_dr = f.get_section_table() + i;
        shared_ptr<Section> sec = make_shared<Section>(cur_sec_dr, i, file_data, sec_strn_table);
        sec_vec_.push_back(sec);
    }

    report(RL_FOUR, "section vector construction completed");
}

vector<shared_ptr<Section> > SectionVec::merge_sections()
{
    vector<shared_ptr<Section> > res;

    shared_ptr<Section> section_text = get_section_by_name(".text");
    vector<shared_ptr<Section> >::iterator it;

    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->flags_ & SHF_MERGE && (*it)->name_ != ".comment") {
            (*(it-1))->_merge_section(*it);
            res.push_back(*it);
            sec_vec_.erase(it);
            it--;
        }
        else if ((*it)->flags_ & SHF_EXECINSTR && !_skip_Xsec((*it)->name_)) {
            section_text->_merge_section(*it);
            res.push_back(*it);
            sec_vec_.erase(it);
            it--;
        }
    }

    report(RL_FOUR, "merging sections completed");
    return res;
}

shared_ptr<Section> SectionVec::get_section_by_name(const string &s)
{
    vector<shared_ptr<Section> >::iterator it;
    shared_ptr<Section> res;
    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->name_ == s)
            return *it;
    }

    return res;
}

shared_ptr<Section> SectionVec::get_section_by_index(UINT16 index)
{
    vector<shared_ptr<Section> >::iterator it;
    shared_ptr<Section> res;
    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->get_section_index() == index)
            return *it;
    }

    return res;
}

bool SectionVec::_skip_Xsec(const string &s)
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
    os << sec.name_ << std::endl;
    string temp("temp");
    std::ofstream of;
    of.open(temp+sec.name_, std::ofstream::binary);
    of.write(reinterpret_cast<const char*>(sec.data_), sec.size_);
    of.close();

    return os;
}
