/*
 * =====================================================================================
 *
 *       Filename:  relocation.cc
 *
 *    Description:  implementation of relocation related classes
 *
 *        Version:  1.0
 *        Created:  11/20/2015 16:24:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "relocation.h"

#include "file.h"
#include "section.h"
#include "symbol.h"
#include "log.h"

Relocation::Relocation(Elf32_Rel *rel, const FileRel &f, const SectionVec &obj_sec_vec, const SectionVec &ms, UINT32 sec_index, const SymbolVec &obj_sym_vec):
    offset_(rel->r_offset),
    type_(rel->r_info & 0xff),
    value_(0),
    addend_(0)
{
    shared_ptr<Section> section;
    section = obj_sec_vec.get_section_by_index(sec_index);
    if (section)
        sec_ = section;
    else {
        section = ms.get_section_by_index(sec_index);
        if (!section)
            report(RL_ONE, "relocation related section %d not found", sec_index);
        sec_ = section;
        offset_ += sec_->get_section_delta();
    }

    shared_ptr<Symbol> symbol;
    symbol = obj_sym_vec.get_symbol_by_index(rel->r_info >> 8);
    if (!symbol)
        report(RL_ONE, "relocation related symbol %d not found", (rel->r_info >> 8));
    sym_ = symbol;
    _markGOTPLTSymbol();
}

void Relocation::_markGOTPLTSymbol()
{
    if (sym_->get_symbol_shndx() != SHN_UNDEF)
        return;
    if (type_ == R_386_GOT32) {
        if (!(sym_->get_symbol_sd_type() & SYM_GOT))
            sym_->add_symbol_sd_type(SYM_GOT);
    }
    else if (type_ == R_386_PLT32) {
        if (!(sym_->get_symbol_sd_type() & SYM_PLT))
            sym_->add_symbol_sd_type(SYM_PLT);
    }
}

void RelocationVec::init(const FileRel &f, SectionVec &obj_sec_vec, const SectionVec &ms, const SymbolVec &obj_sym_vec)
{
    const vector<UINT32> rel_indexes = f.get_relocation_index();
    for (int i = 0; i < rel_indexes.size(); i++) {
        Elf32_Shdr *rel_sec_dr = f.get_section_table() + rel_indexes[i];
        int rel_entry_number = rel_sec_dr->sh_size / sizeof(Elf32_Rel);
        Elf32_Rel *rel_table = reinterpret_cast<Elf32_Rel*>(f.get_file_data() + rel_sec_dr->sh_offset);

        for (int j = 0; j < rel_entry_number; j++) {
            shared_ptr<Relocation> rel = make_shared<Relocation>(rel_table + j, f, obj_sec_vec, ms, rel_sec_dr->sh_info, obj_sym_vec);
            rel_vec_.push_back(rel);
        }

        obj_sec_vec.delete_section_by_index(rel_indexes[i]);
    }

    report(RL_FOUR, "complete relocation list construction");
}

/*-----------------------------------------------------------------------------
 *  helper printer functions below
 *-----------------------------------------------------------------------------*/
ostream &operator<<(ostream &os, const Relocation &rel)
{
    os << std::hex;
    os << rel.offset_ << "\t";
    os << rel.type_ << "\t";
    // type_ can't be seen
    os << rel.sym_->get_symbol_index() << "\t";
    os << rel.sym_->get_symbol_name();
    os << std::endl;
    
    return os;
}

ostream &operator<<(ostream &os, const RelocationVec &r)
{
    for (int i = 0; i < r.rel_vec_.size(); i++)
        os << i << " " << *(r.rel_vec_[i]);
    return os;
}
