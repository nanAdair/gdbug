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
#include <cmath>
#include <algorithm>
#include <fstream>
#include <map>

#include "sectionGD.h"
#include "file.h"
#include "symbol.h"
#include "log.h"
using namespace std;

UINT32 _calculate_hash(const string &name);

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

UINT32 Section::get_section_score() const
{
    UINT32 n;
    n = misc_ ? 50 : 0;
    n += get_section_index();
    
    /* NULL section: 0 */
    if (type_ == SHT_NULL)
        return 0;
    
    /* .interp 1 */
    if (name_ == INTERP_SECTION_NAME)
        return 1;
    
    /* .note.ABI-tag 2 */
    if (name_ == NOTE_SECTION_NAME)
        return 2;

    /* A: +1000 */
    if (flags_ == SHF_ALLOC)
        return 1000 + n;
    
    /* AX: +1000*/
    if (flags_ == (SHF_ALLOC | SHF_EXECINSTR))
        return 1000 + n;
    
    /* WA: + 2000 */
    if (flags_ == (SHF_ALLOC | SHF_WRITE))
        return 2000 + n;
    
    /* STR: +3000 */
    if (flags_ == SHF_STRINGS)
        return 3000 + n;
    
    /* Symbol Table: +7000 */
    if (type_ == SHT_SYMTAB)
        return 7000 + n;
    
    /* String Table: +8000 */
    if (type_ == SHT_STRTAB)
        return 8000 + n;
    
    return 6000 + n;
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

int SectionDynstr::find_offset(const string &name)
{
    UINT32 offset = 1;
    while (offset < size_) {
        string tmp(reinterpret_cast<char*>(data_ + offset));
        if (tmp == name)
            break;
        offset += tmp.size() + 1;
    }
    return offset < size_ ? offset : -1;
}

void SectionInterp::set_interp_data(const string &ld_file)
{
    expand_section_data(reinterpret_cast<const UINT8*>(ld_file.c_str()), ld_file.size()+1, 1);
}

void SectionHash::set_hash_data(const SymbolDynVec &dsl)
{
    /* dsl size plus one empty symbol */
    int number = dsl.get_dynsym_vec_size() + 1;
    
    int nbuckets, nchains;
    nchains = number;
    nbuckets = (int)log2(number) + 1;
    int datasize = (1 + 1 + nbuckets + nchains) * 4;

    UINT8 *buffer;
    buffer = new UINT8[datasize];
    memset(buffer, 0x0, datasize);
    memcpy(buffer, &nbuckets, sizeof(int));
    memcpy(buffer + 4, &nchains, sizeof(int));

    for (int i = number-2; i >= 0; i--) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        int hash_value = _calculate_hash(dynsym->get_symbol_name());
        int order, index, symbol_index;
        order = hash_value % nbuckets;
        index = *(int *)((buffer + 8 + order * 4));
        // add 1 for the empty dynamic symbol
        symbol_index = dynsym->get_symbol_index() + 1;
        if (index == 0)
            memcpy(buffer + 8 + order * 4, &symbol_index, sizeof(int));
        else {
            order = index;
            index = *(int *)((buffer + 8 + nbuckets * 4 + order * 4));
            while (index) {
                order = index;
                index = *(int *)((buffer + 8 + nbuckets * 4 + order * 4));
            }
            memcpy(buffer + 8 + nbuckets * 4 + order * 4, &symbol_index, 4);
        }
    }
    
    expand_section_data(buffer, datasize, 1);
    delete [] buffer;
}

void SectionGnuVersion::set_gnuversion_data(const SymbolDynVec &dsl)
{
    UINT16 version = 0;
    expand_section_data(reinterpret_cast<UINT8*>(&version), sizeof(version), 1);
    for (UINT32 i = 0; i < dsl.get_dynsym_vec_size(); i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        version = dynsym->get_dynsym_version();
        expand_section_data(reinterpret_cast<UINT8*>(&version), sizeof(version), 1);
    }
}

void SectionGnuVersionR::set_gnuversionr_data(const SymbolDynVec& dsl, const vector<string> &so_files, shared_ptr<Section> &dynstr)
{
    for (int i = 0; i < so_files.size(); i++) {
        int ver_number = 0;
        map<string, UINT16> ver;
        map<string, UINT16>::iterator vit;

        for (UINT32 j = 0; j < dsl.get_dynsym_vec_size(); j++) {
            shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(j);
            UINT16 version = dynsym->get_dynsym_version();  
            string version_name = dynsym->get_dynsym_version_name(); 
            string file_name = dynsym->get_dynsym_file();
            if (version == 0 || 
                    version_name == "" ||
                    file_name!= so_files[i])
                continue;

            vit = ver.find(version_name);
            if (vit == ver.end()) {
                ver[version_name] = version;
                ver_number++;
            }
        } 

        //TODO: if ver_number == 0 what to do
        Elf32_Verneed verneed;
        verneed.vn_version = 1;
        verneed.vn_cnt = ver_number;
        verneed.vn_file = std::dynamic_pointer_cast<SectionDynstr>(dynstr)->find_offset(so_files[i]);
        verneed.vn_aux = 0x10;
        /* Add the size of the Elf32_Verneed itself */
        verneed.vn_next = (i == so_files.size() - 1) ? 0 : sizeof(Elf32_Vernaux) * ver_number + 0x10;
        expand_section_data(reinterpret_cast<UINT8*>(&verneed), sizeof(Elf32_Verneed), 1);

        for (vit = ver.begin(); vit != ver.end(); ++vit) {
            Elf32_Vernaux vernaux;
            string version_name;
            UINT16 version_number;
            version_name = vit->first;
            version_number = vit->second;
            
            vernaux.vna_hash = _calculate_hash(version_name);
            vernaux.vna_flags = 0;
            vernaux.vna_other = version_number;
            vernaux.vna_name = std::dynamic_pointer_cast<SectionDynstr>(dynstr)->find_offset(version_name);
            
            if ((++vit) == ver.end())
                vernaux.vna_next = 0;
            else
                vernaux.vna_next = 0x10;
            --vit;
            expand_section_data(reinterpret_cast<UINT8*>(&vernaux), sizeof(Elf32_Vernaux), 1);
        }
    }
}

void SectionDynsym::set_dynsym_data(const SymbolDynVec &dsl)
{
    UINT32 size = dsl.get_dynsym_vec_size();
    Elf32_Sym empty;
    memset(&empty, 0x0, sizeof(empty));
    expand_section_data(reinterpret_cast<UINT8*>(&empty), sizeof(empty), 1);
    for (UINT32 i = 0; i < size; i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        Elf32_Sym temp = dynsym->symbol_to_elfsym();
        expand_section_data(reinterpret_cast<UINT8*>(&temp), sizeof(temp), 1);
    }
}

void SectionPlt::_add_plt_head()
{
    PLT_Instr instr[2];
    int i, offset;
    
    UINT8 *buffer = new UINT8 [plt_item_size];
    memset(buffer, 0x0, plt_item_size);
    
    instr[0].opcode = 0x35ff;
    instr[0].oprand = 0x0;
    instr[1].opcode = 0x25ff;
    instr[1].oprand = 0x0;
    
    offset = 0;
    for (i = 0; i < 2; i++) {
        memcpy(buffer + offset, &(instr[i].opcode), 0x2);
        offset += 2;
        memcpy(buffer + offset, &(instr[i].oprand), 0x4);
        offset += 4;
    }

    expand_section_data(buffer, plt_item_size, 1);
    delete [] buffer;
}

void SectionPlt::set_plt_data(const SymbolDynVec &dsl)
{
    _add_plt_head();
    int n = 0;
    for (UINT32 i = 0; i < dsl.get_dynsym_vec_size(); i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        if (!(dynsym->get_symbol_sd_type() & SYM_PLT))
            continue;

        PLT_Instr instr[3];
        int offset = 0;
        
        instr[0].opcode = 0x25ff;
        instr[0].oprand = 0x0;
        instr[1].opcode = 0x68;
        instr[1].oprand = n * 0x8;
        instr[2].opcode = 0xe9;
        instr[2].oprand = 0x0;
        
        UINT8 *buffer = new UINT8 [plt_item_size];
        memset(buffer, 0x0, plt_item_size);

        memcpy(buffer + offset, &(instr[0].opcode), 2);
        offset += 2;
        memcpy(buffer + offset, &(instr[0].oprand), 4);
        offset += 4;
        
        for (int i = 1; i < 3; i++) {
            memcpy(buffer + offset, &(instr[i].opcode), 1);
            offset += 1;
            memcpy(buffer + offset, &(instr[i].oprand), 4);
            offset += 4;
        }
        expand_section_data(buffer, plt_item_size, 1);
        delete [] buffer;
        n++;
    }
}

void SectionGotPlt::set_got_plt_data(const SymbolDynVec &dsl)
{
    int number = 0;
    for (UINT32 i = 0; i < dsl.get_dynsym_vec_size(); i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        if (dynsym->get_symbol_sd_type() & SYM_PLT)
            number++;
    }

    // size plus 0xc for the head addresses
    int size = 0x4 * (number + 3);
    UINT8 *buffer = new UINT8 [size];
    memset(buffer, 0x0, size);
    expand_section_data(buffer, size, 1);
    delete [] buffer;
}

void SectionRelPlt::set_rel_plt_data(const SymbolDynVec &dsl)
{
    int index = 0;
    for (UINT32 i = 0; i < dsl.get_dynsym_vec_size(); i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        if (!(dynsym->get_symbol_sd_type() & SYM_PLT))
            continue;

        Elf32_Rel rel;
        rel.r_offset = 0xc + 0x4 * index;
        rel.r_info = ((dynsym->get_symbol_index() + 1) << 8) + R_386_JMP_SLOT;

        expand_section_data(reinterpret_cast<UINT8*>(&rel), sizeof(Elf32_Rel), 1);
        index++;
    }
}

void SectionGot::set_got_data(const SymbolDynVec &dsl)
{
    int number = 0;
    for (UINT32 i = 0; i < dsl.get_dynsym_vec_size(); i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        if (dynsym->get_symbol_sd_type() & SYM_GOT) 
            number++;
    }

    int size = 0x4 * number;
    UINT8 *buffer = new UINT8 [size];
    memset(buffer, 0x0, size);
    expand_section_data(buffer, size, 1);
    delete [] buffer;
}

void SectionRelDyn::set_rel_dyn_data(const SymbolDynVec &dsl)
{
    int index = 0;
    for (UINT32 i = 0; i < dsl.get_dynsym_vec_size(); i++) {
        shared_ptr<SymbolDyn> dynsym = dsl.get_ith_dynsym(i);
        if (!(dynsym->get_symbol_sd_type() & SYM_GOT))
            continue;

        Elf32_Rel rel;
        rel.r_offset = 0x4 * index;
        rel.r_info = ((dynsym->get_symbol_index() + 1) << 8) + R_386_GLOB_DAT;

        expand_section_data(reinterpret_cast<UINT8*>(&rel), sizeof(Elf32_Rel), 1);
        index++;
    }
}

void SectionDynamic::set_dynamic_data(int num)
{
    UINT32 datasize = DYNAMIC_ENTSIZE * (DYNAMIC_NUMBER + num);
    UINT8 *buffer = new UINT8 [datasize];
    memset(buffer, 0x0, datasize);
    expand_section_data(buffer, datasize, 1);
    delete [] buffer;
}

void SectionShstr::set_shstr_data(const string &data)
{
    /* destroy original data */
    delete [] data_;
    data_ = NULL;
    size_ = 0;

    expand_section_data(reinterpret_cast<const UINT8*>(data.c_str()), data.size()+1, 1);
    for (int i = 0; i < size_; i++)
        if (data_[i] == ';')
            data_[i] = 0;
}

int SectionShstr::find_section_name_offset(const string &name)
{
    int res = -1;
    UINT32 offset = 1;
    while (offset < size_) {
        string cur_name = string(reinterpret_cast<char*>(data_+offset));
        if (cur_name == name) {
            res = offset;
            break;
        }
        offset = offset + cur_name.size() + 1;
    }
    return res;
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
        else if (sec_name == ".shstrtab")
            sec = make_shared<SectionShstr>(cur_sec_dr, i, file_data, sec_strn_table);
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

void SectionVec::delete_section_by_index(UINT16 index)
{
    vector<shared_ptr<Section> >::iterator it;
    shared_ptr<Section> res;
    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) {
        if ((*it)->get_section_index() == index)
            sec_vec_.erase(it);
    }
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
    shared_ptr<Section> dynstr, init, shstr;
    dynstr = get_section_by_name(DYNSTR_SECTION_NAME);
    init = get_section_by_name(INIT_SECTION_NAME);
    shstr = get_section_by_name(SHSTRTAB_SECTION_NAME);

    std::dynamic_pointer_cast<SectionDynstr>(dynstr)->set_dynstr_data(dsl, so_files);

    string section_names = _accumulate_names();

    std::dynamic_pointer_cast<SectionShstr>(shstr)->set_shstr_data(section_names);
    cout << *shstr;
    
    vector<shared_ptr<Section> >::iterator it;
    for (it = sec_vec_.begin()+1; it != sec_vec_.end(); it++) {
        string name = (*it)->get_section_name();
        int offset = std::dynamic_pointer_cast<SectionShstr>(shstr)->find_section_name_offset(name);
        if (offset == -1)
            report(RL_ONE, "section name can't be found");
        (*it)->set_section_name_offset(offset);

        if (name == INTERP_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionInterp>(*it)->set_interp_data(ld_file);
        else if (name == DYNSYM_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionDynsym>(*it)->set_dynsym_data(dsl);
        else if (name == HASH_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionHash>(*it)->set_hash_data(dsl);
        else if (name == GV_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionGnuVersion>(*it)->set_gnuversion_data(dsl);
        else if (name == GNR_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionGnuVersionR>(*it)->set_gnuversionr_data(dsl, so_files, dynstr);
        else if (name == PLT_SECTION_NAME) {
            std::dynamic_pointer_cast<SectionPlt>(*it)->set_plt_data(dsl);
            (*it)->misc_ = init->misc_;
            (*it)->origin_index_ = init->get_section_index();
        }
        else if (name == GOT_PLT_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionGotPlt>(*it)->set_got_plt_data(dsl);
        else if (name == REL_PLT_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionRelPlt>(*it)->set_rel_plt_data(dsl);
        else if (name == GOT_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionGot>(*it)->set_got_data(dsl);
        else if (name == REL_DYN_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionRelDyn>(*it)->set_rel_dyn_data(dsl);
        else if (name == DYNAMIC_SECTION_NAME) 
            std::dynamic_pointer_cast<SectionDynamic>(*it)->set_dynamic_data(so_files.size());
    }
}

void SectionVec::allocate_address()
{
    _sort_sections();

    UINT32 base_addr, offset;
    base_addr = 0x8048000;
    offset = 0x114;
    UINT32 addend = 0;

    vector<shared_ptr<Section> >::iterator it;
    for (it = sec_vec_.begin()+1; it != sec_vec_.end(); it++) {
        UINT32 align = (*it)->get_section_addralign();
        int addition = 0;
        while ((offset + addition) % align)
            addition++;

        if (addition) {
            UINT8 *buffer = new UINT8 [addition];
            memset(buffer, 0x0, addition);
            (*it)->expand_section_data(buffer, addition, 0);
            delete [] buffer;
            offset += addition;
        }

        if ((*it)->get_section_flags() & SHF_WRITE)
            addend = 0x1000;
        if ((*it)->get_section_flags() & SHF_ALLOC)
            (*it)->set_section_address(base_addr + offset + addend);
        else
            (*it)->set_section_address(0);

        (*it)->set_section_file_offset(offset);
        if ((*it)->get_section_name() != BSS_SECTION_NAME)
            offset += (*it)->get_section_size();
    }
}

void SectionVec::_sort_sections()
{
    vector<shared_ptr<Section> >::iterator it;
    for (it = sec_vec_.begin(); it != sec_vec_.end(); it++) 
        (*it)->set_section_misc((*it)->get_section_score());
    
    sort(sec_vec_.begin(), sec_vec_.end());
    for (int i = 0; i < sec_vec_.size(); i++)
        sec_vec_[i]->set_section_final_index(i);
}

string SectionVec::_accumulate_names() const
{
    string res = "";
    vector<shared_ptr<Section> >::const_iterator it;
    for (it = sec_vec_.begin()+1; it != sec_vec_.end(); it++) {
        string name = (*it)->name_;
        res = res + ";" + name;
    }
    return res;
}

UINT32 _calculate_hash(const string &name)
{
    UINT32 h = 0, g;
    
    for (int i = 0; i < name.size(); i++){
        h = (h << 4) + static_cast<UINT32>(name[i]);
        if (g = h & 0xf0000000)
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
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
    os << sec.get_section_index() << " ";
    os << sec.name_ << " ";
    os << sec.address_ << " " << sec.file_offset_ << " " << sec.size_ << std::endl;
    string temp("temp");
    std::ofstream of;
    of.open(temp+sec.name_, std::ofstream::binary);
    of.write(reinterpret_cast<const char*>(sec.data_), sec.size_);
    of.close();

    return os;
}

bool operator<(shared_ptr<Section> a, shared_ptr<Section> b)
{
    return a->get_section_misc() < b->get_section_misc();
}
