/*
 * =====================================================================================
 *
 *       Filename:  symbol.cc
 *
 *    Description:  implementation of symbol related classes
 *
 *        Version:  1.0
 *        Created:  11/20/2015 14:54:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "symbol.h"

#include <set>
using std::set;

#include "file.h"
#include "section.h"
#include "version.h"
#include "log.h"

Symbol::Symbol(Elf32_Sym *sym, UINT32 index):
    name_offset_(sym->st_name),
    value_(sym->st_value),
    size_(sym->st_size),
    type_(sym->st_info & 0xf),
    binding_(sym->st_info >> 4),
    other_(sym->st_other),
    shndx_(sym->st_shndx),
    index_(index),
    sd_type_(SYM_LOCAL),
    handled_(0) {}

Symbol::Symbol(Elf32_Sym *sym, UINT32 index, UINT8 *sym_strn_table, const SectionVec &obj_sec_vec, const SectionVec &ms):
    Symbol(sym, index)
{
    name_ = string(reinterpret_cast<char *>(sym_strn_table + name_offset_));

    if (shndx_ != SHN_ABS && shndx_ != SHN_COMMON && shndx_ != SHN_UNDEF) {
        shared_ptr<Section> target = obj_sec_vec.get_section_by_index(shndx_);
        if (target)
            sec_ = target;
        else {
            target = ms.get_section_by_index(shndx_);
            if (!target)
                report(RL_ONE, "symbol %s related section %d not found", name_, shndx_);
            sec_ = target->get_merged_section();
            value_ += target->get_section_delta();
            shndx_ = sec_->get_section_index();
        }
    }
}

void Symbol::_handleCOMMON(const SectionVec &obj_sec_vec)
{
    shared_ptr<Section> bss = obj_sec_vec.get_section_by_name(".bss");
    UINT32 addition, datasize, newdatasize, sym_size, addralign;
    addition = 0;
    datasize = bss->get_section_size();
    sym_size = size_;

    addralign = bss->get_section_addralign();
    addralign = sym_size > addralign ? sym_size : addralign;
    while ((datasize + addition) % addralign)
        addition++;
    newdatasize = datasize + addition + sym_size;

    bss->set_section_size(newdatasize);
    bss->set_section_addralign(addralign);

    value_ = datasize + addition;
    sec_ = bss;
}

Elf32_Sym Symbol::symbol_to_elfsym() const
{
    Elf32_Sym res;
    res.st_name = name_offset_;
    res.st_value = value_;
    res.st_size = size_;
    res.st_info = (binding_ << 4) + (type_ & 0xf);
    res.st_other = other_;
    res.st_shndx = shndx_;

    return res;
}

void SymbolVec::init(const FileRel &f, const SectionVec &obj_sec_vec, const SectionVec &ms)
{
    Elf32_Sym *cur_sym_dr;
    Elf32_Shdr *sym_table_dr;
    UINT8 *sym_strn_table;

    sym_table_dr = f.get_relsym_table_dr();
    sym_strn_table = f.get_relsym_strn_table();

    int symbol_number = sym_table_dr->sh_size / sym_table_dr->sh_entsize;
    for (int i = 0; i < symbol_number; i++) {
        cur_sym_dr = f.get_relsym_table() + i;
        shared_ptr<Symbol> sym = make_shared<Symbol>(cur_sym_dr, i, sym_strn_table, obj_sec_vec, ms);
        
        if (sym->shndx_ == SHN_COMMON)
            sym->_handleCOMMON(obj_sec_vec);
        sym_vec_.push_back(sym);
   }

    report(RL_FOUR, "complete symbol list construction");
}

shared_ptr<Symbol> SymbolVec::get_symbol_by_index(UINT32 index) const 
{
    vector<shared_ptr<Symbol> >::const_iterator it;
    shared_ptr<Symbol> res;
    for (it = sym_vec_.begin(); it != sym_vec_.end(); it++) {
        if ((*it)->index_ == index) {
            res = *it;
            break;
        }
    }
    return res;
}

SymbolDyn::SymbolDyn(Elf32_Sym *sym, UINT32 index, UINT8 *dynsym_strn_table, const SectionVec &dsl, string file_name, const VersionVec &version_vec):
    Symbol(sym, index),
    file_(file_name)
{
    name_ = string(reinterpret_cast<char *>(dynsym_strn_table + name_offset_));
    
    shared_ptr<Section> gnu_version = dsl.get_section_by_name(".gnu.version");
    version_ = std::dynamic_pointer_cast<SectionGnuVersion>(gnu_version)->get_version_number(index_);
    version_name_ = version_vec.get_version_name(version_);
}

SymbolDynVec::SymbolDynVec(const FileDyn &file, const SectionVec &so_sec_vec, const VersionVec &version_vec)
{
    Elf32_Sym *cur_sym_dr;
    Elf32_Shdr *dynsym_table_dr;
    UINT8 *dynsym_strn_table;
    string file_name;

    dynsym_table_dr = file.get_dynsym_table_dr();
    dynsym_strn_table = file.get_dynsym_strn_table();
    file_name = file.get_file_name();

    for (int i = 1; i < dynsym_table_dr->sh_size / dynsym_table_dr->sh_entsize; i++) {
        cur_sym_dr = file.get_dynsym_table() + i;
        shared_ptr<SymbolDyn> sym = make_shared<SymbolDyn>(cur_sym_dr, i, dynsym_strn_table, so_sec_vec, file_name, version_vec);
        dynsym_vec_.push_back(sym);
    }
}

shared_ptr<SymbolDyn> SymbolDynVec::get_dynsym_by_name(string name) const
{
    vector<shared_ptr<SymbolDyn> >::const_iterator it;
    shared_ptr<SymbolDyn> res;
    for (it = dynsym_vec_.begin(); it != dynsym_vec_.end(); it++) {
        if ((*it)->get_symbol_name() == name) {
            res = *it;
            break;
        }
    }
    return res;
}

void SymbolDynVec::addFromSDVec(const SymbolVec &obj_sym_vec, const SymbolDynVec &so_sym_vec)
{
    _markDynSymbol(obj_sym_vec, so_sym_vec);

    vector<shared_ptr<Symbol> >::const_iterator it;
    vector<shared_ptr<SymbolDyn> >::const_iterator dit;
    int index = dynsym_vec_.size();

    for (it = obj_sym_vec.sym_vec_.begin(); it != obj_sym_vec.sym_vec_.end(); it++) {
        if ((*it)->get_symbol_sd_type() == SYM_LOCAL || (*it)->get_symbol_handle())
            continue;

        shared_ptr<SymbolDyn> new_sym = make_shared<SymbolDyn> ((*it).get());
        if ((*it)->get_symbol_shndx() == SHN_UNDEF) {
            shared_ptr<SymbolDyn> target = so_sym_vec.get_dynsym_by_name(new_sym->get_symbol_name());
            if (target) {
                UINT16 version = target->get_dynsym_version();
                new_sym->set_dynsym_version(version);
                if (version)
                    new_sym->set_dynsym_version_name(target->get_dynsym_version_name());
                new_sym->set_dynsym_file(target->get_dynsym_file());
            }
        }
        else {
            new_sym->set_dynsym_version(1);
        }
        new_sym->set_symbol_index(index++);
        (*it)->set_symbol_handle(1);
        dynsym_vec_.push_back(new_sym);
    }
}

void SymbolDynVec::_markDynSymbol(const SymbolVec &obj_sym_vec, const SymbolDynVec &so_sym_vec)
{
    vector<shared_ptr<Symbol> >::const_iterator it;
    vector<shared_ptr<SymbolDyn> >::const_iterator dit;

    for (dit = so_sym_vec.dynsym_vec_.begin(); dit != so_sym_vec.dynsym_vec_.end(); dit++) {
        for (it = obj_sym_vec.sym_vec_.begin(); it != obj_sym_vec.sym_vec_.end(); it++) {
            UINT32 dynsym_type = (*dit)->get_symbol_type();
            UINT32 objsym_type = (*it)->get_symbol_type();
            if ((*dit)->get_symbol_name() == (*it)->get_symbol_name() && 
                    (*it)->get_symbol_sd_type() == SYM_LOCAL &&
                    !(*it)->get_symbol_handle()) {
                if (objsym_type == STT_SECTION)
                    break;

                if (dynsym_type == STT_NOTYPE &&
                        (*it)->get_symbol_name() == "__gmon_start__") 
                    (*it)->add_symbol_sd_type(SYM_OUT);
                else if ((*it)->get_symbol_shndx() != SHN_UNDEF) 
                    (*it)->add_symbol_sd_type(SYM_OUT);
                else if (dynsym_type == STT_FUNC || dynsym_type == STT_GNU_IFUNC) {
                    (*it)->add_symbol_sd_type(SYM_PLT);
                    (*it)->set_symbol_type(dynsym_type);
                }
                else
                    (*it)->add_symbol_sd_type(SYM_GOT);
                (*it)->del_symbol_sd_type(SYM_LOCAL);
                break;
            }
        }
    }
}

// usr ; as the delimiter to get names together
string SymbolDynVec::accumulate_names(UINT32 offset) const
{
    string res = "";
    vector<shared_ptr<SymbolDyn> >::const_iterator it;
    set<string> versions;
    
    for (it = dynsym_vec_.begin(); it != dynsym_vec_.end(); it++) {
        string name = (*it)->get_symbol_name();
        if (res == "")
            res = name;
        else
            res = res + ";" + name;
        (*it)->set_symbol_name_offset(offset);
        offset += name.size() + 1;

        string version_name = (*it)->get_dynsym_version_name();
        if (version_name != "" && (versions.find(version_name) == versions.end()))
            versions.insert(version_name);
    }

    for (set<string>::iterator vit = versions.begin(); vit != versions.end(); vit++) {
        res = res + ";" + *vit;
    }

    return res;
}

UINT32 SymbolDynVec::get_dynsym_vec_size() const 
{
    return dynsym_vec_.size();
}

shared_ptr<SymbolDyn> SymbolDynVec::get_ith_dynsym(UINT32 i) const
{
    return dynsym_vec_[i];
}

/*-----------------------------------------------------------------------------
 *  helper printer functions
 *-----------------------------------------------------------------------------*/
ostream& operator<<(ostream & os, const Symbol &sym)
{
    if (sym.sec_)
        os << sym.sec_->get_section_name();
    else
        os << "\t";
    os << " " << sym.index_ << " " << sym.name_ << std::endl;
    return os;
}

ostream& operator<<(ostream &os, const SymbolVec &s)
{
    for (int i = 0; i < s.sym_vec_.size(); i++)
        os << i << " " << *(s.sym_vec_[i]);
    return os;
}

ostream& operator<<(ostream & os, const SymbolDyn &sym)
{
    if (sym.sec_)
        os << sym.sec_->get_section_name();
    else
        os << "\t";
    os << sym.index_ << " " << sym.name_offset_ << " " << sym.name_;
    os << " " << sym.version_name_ << std::endl;
    return os;
}

ostream& operator<<(ostream &os, const SymbolDynVec &s)
{
    for (int i = 0; i < s.dynsym_vec_.size(); i++)
        os << i << " " << *(s.dynsym_vec_[i]);
    return os;
}
