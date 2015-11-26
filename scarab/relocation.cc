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

#include <iostream>
using namespace std;

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
        sec_ = section->get_merged_section();
        offset_ += sec_->get_section_delta();
    }

    addend_ = *(reinterpret_cast<int*>(sec_->get_section_data()) + offset_);

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

void Relocation::apply_relocation_32()
{
    if (sym_->get_symbol_sd_type() & SYM_GOT)
        report(RL_ONE, "error: can't handle got symbol for R_386_32");

    value_ = sym_->get_symbol_value();
    int result = static_cast<int>(value_) + addend_;

    sec_->update_section_data(offset_, reinterpret_cast<UINT8*>(&result), sizeof(int));
}

void Relocation::apply_relocation_pc32(const SectionVec &obj_sec_vec, const SymbolDynVec &dsl)
{
    if (sym_->get_symbol_sd_type() & SYM_PLT) {
        shared_ptr<Section> plt = obj_sec_vec.get_section_by_name(PLT_SECTION_NAME);
        shared_ptr<SymbolDyn> same_dynsym = dsl.get_dynsym_by_name(sym_->get_symbol_name());
        value_ = same_dynsym->get_dynsym_plt_index() * 0x10 + plt->get_section_address();
    }
    else
        value_ = sym_->get_symbol_value();

    int result = static_cast<int>(value_) + addend_ - (sec_->get_section_address() + offset_);

    sec_->update_section_data(offset_, reinterpret_cast<UINT8*>(&result), sizeof(int));
}

void Relocation::apply_relocation_gotpc()
{
    value_ = sym_->get_symbol_value();

    int result = static_cast<int>(value_) + addend_ - (sec_->get_section_address() + offset_);

    sec_->update_section_data(offset_, reinterpret_cast<UINT8*>(&result), sizeof(int));
}


void Relocation::apply_relocation_gotoff(const SectionVec &obj_sec_vec)
{
    value_ = sym_->get_symbol_value();

    shared_ptr<Section> gotplt = obj_sec_vec.get_section_by_name(GOT_PLT_SECTION_NAME);
    int result = static_cast<int>(value_) - gotplt->get_section_address();

    sec_->update_section_data(offset_, reinterpret_cast<UINT8*>(&result), sizeof(int));
}

void Relocation::apply_relocation_got32(const SectionVec &obj_sec_vec, const SymbolDynVec &dsl)
{
    shared_ptr<Section> gotplt = obj_sec_vec.get_section_by_name(GOT_PLT_SECTION_NAME);
    shared_ptr<Section> got = obj_sec_vec.get_section_by_name(GOT_SECTION_NAME);

    shared_ptr<SymbolDyn> same_dynsym = dsl.get_dynsym_by_name(sym_->get_symbol_name());
    UINT32 index = same_dynsym->get_dynsym_got_index();
    UINT32 got_num = got->get_section_size() / got->get_section_entsize();

    value_ = gotplt->get_section_address() + (-4) * (got_num - index);
    int result = static_cast<int>(value_) - gotplt->get_section_address();

    sec_->update_section_data(offset_, reinterpret_cast<UINT8*>(&result), sizeof(int));
}

void Relocation::apply_relocation_plt32(const SectionVec &obj_sec_vec, const SymbolDynVec &dsl)
{
    apply_relocation_pc32(obj_sec_vec, dsl);
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

void RelocationVec::apply_relocations(const SectionVec &obj_sec_vec, const SymbolDynVec &dsl)
{
    vector<shared_ptr<Relocation> >::iterator it;
    for (it = rel_vec_.begin(); it != rel_vec_.end(); it++) {
        switch((*it)->get_relocation_type()) {
            case R_386_NONE:
                break;
            case R_386_32:
                (*it)->apply_relocation_32();
                break;
            case R_386_PC32:
                (*it)->apply_relocation_pc32(obj_sec_vec, dsl);
                break;
            case R_386_GOTPC:
                (*it)->apply_relocation_gotpc();
                break;
            case R_386_GOTOFF:
                (*it)->apply_relocation_gotoff(obj_sec_vec);
                break;
            case R_386_GOT32:
                (*it)->apply_relocation_got32(obj_sec_vec, dsl);
                break;
            case R_386_PLT32:
                (*it)->apply_relocation_plt32(obj_sec_vec, dsl);
                break;
            default:
                report(RL_ONE, "error: can't handle the relocation type");
        }
    }

    report(RL_FOUR, "complete apply relocations");
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
