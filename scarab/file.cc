/*
 * =====================================================================================
 *
 *       Filename:  file.cc
 *
 *    Description:  implementation of file related classes
 *
 *        Version:  1.0
 *        Created:  11/16/2015 11:02:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "file.h"

#include <fstream>
using std::endl;

#include "log.h"


File::File(const string &file_name, const BINARY_TYPE type):
    file_name_(file_name),
    file_type_(type),
    file_data_(NULL)
{
    _initPartFile();
    _constructFileMeta();
}

File::~File()
{
    if (file_data_) {
        delete[] file_data_;
        file_data_ = NULL;
    }
}

void File::_initPartFile()
{
    std::ifstream ifs;
    ifs.open(file_name_.c_str(), std::ifstream::binary);
    if (!ifs)
        report(RL_ONE, "error: can't open file %s", file_name_.c_str());

    ifs.seekg(0, ifs.end);
    UINT32 file_size = ifs.tellg();

    ifs.seekg(0, ifs.beg);
    UINT8 *file_data = new UINT8 [file_size];
    if (!file_data) 
        report(RL_ONE, "error: allocate %d bytes memory for file %s failed", file_size, file_name_.c_str());

    ifs.read(reinterpret_cast<char *>(file_data), file_size);
    if (ifs)
        report(RL_FOUR, "all data read successfully");
    else
        report(RL_ONE, "error: only %d data read", ifs.gcount());
    ifs.close();

    file_size_ = file_size;
    file_data_ = file_data;
}

void File::_constructFileMeta()
{
    shared_ptr<FileHeader> file_header = std::make_shared<FileHeader> (reinterpret_cast<Elf32_Ehdr*>(file_data_));

    UINT32 shoff = file_header->get_shoff();
    Elf32_Shdr *section_table = reinterpret_cast<Elf32_Shdr*>(file_data_ + shoff);

    UINT16 shnum = file_header->get_shnum();

    UINT16 shstrndx = file_header->get_shstrndx();
    Elf32_Shdr *section_strn_dr = section_table + shstrndx;
    UINT8 *section_strn_table = file_data_ + section_strn_dr->sh_offset;

    file_header_ = file_header;
    section_table_ = section_table;
    section_strn_table_ = section_strn_table;
    section_number_ = shnum;
}

FileRel::FileRel(const string &file_name):
    File(file_name, File::BINARY_RELOCATABLE_TYPE)
{
    for (int i = 0; i < section_number_; i++) {
        Elf32_Shdr *cur_sec_dr = section_table_ + i;
        if (cur_sec_dr->sh_type == SHT_SYMTAB) {
            relsym_table_dr_ = cur_sec_dr;
            relsym_table_ = reinterpret_cast<Elf32_Sym*>(file_data_ + relsym_table_dr_->sh_offset);
            relsym_strn_table_ = file_data_ + (section_table_ + relsym_table_dr_->sh_link)->sh_offset;
        }

        else if (cur_sec_dr->sh_type == SHT_REL)
            relocation_index_.push_back(i);
    }
}

FileDyn::FileDyn(const string &file_name):
    File(file_name, File::BINARY_SHARED_TYPE)
{
    for (int i = 0; i < section_number_; i++) {
        Elf32_Shdr *cur_sec_dr = section_table_ + i;
        if (cur_sec_dr->sh_type == SHT_DYNSYM) {
            dynsym_table_dr_ = cur_sec_dr;
            dynsym_table_ = reinterpret_cast<Elf32_Sym*>(file_data_ + dynsym_table_dr_->sh_offset);
            dynsym_strn_table_ = file_data_ + (section_table_ + dynsym_table_dr_->sh_link)->sh_offset;
        }
    }
}

/*-----------------------------------------------------------------------------
 *  helper printer functions below
 *-----------------------------------------------------------------------------*/
ostream &operator<<(ostream &os, const FileRel &f)
{
    os << dynamic_cast<const File&>(f);
}

ostream &operator<<(ostream &os, const File &f)
{
    os << "========" << f.file_name_ << "========" << endl;
    os << "File size is " << f.file_size_ << endl;
    os << "Section number is " << f.section_number_ << endl;
    os << *(f.file_header_);
}

ostream &operator<<(ostream &os, const FileHeader &fh)
{
    os << "Elf file header: " << endl;
    os << "  Magic: \t";
    for (int i = 0; i < 16; i++)
        os << std::hex << static_cast<int>(fh.ident_[i]) << " ";
    os << endl;
    os << "  Class: \t\t\t\t" << fh.type_ << endl;
    os << "  Data: \t\t\t\t" << fh.machine_ << endl;
    os << std::dec;
    os << "  Start of section headers: \t\t" << fh.shoff_ << endl;
    os << "  Size of this header: \t\t\t" << fh.ehsize_ << endl;
}
