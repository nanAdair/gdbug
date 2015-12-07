/*
 * =====================================================================================
 *
 *       Filename:  file.h
 *
 *    Description:  classes definition of file related data structure
 *
 *        Version:  1.0
 *        Created:  11/16/2015 09:43:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef GDBUG_SCARAB_FILE_H
#define GDBUG_SCARAB_FILE_H

#include <elf.h>

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <ostream>
using std::ostream;
#include <memory>
using std::shared_ptr;

#include "type.h"

class SectionVec;

class FileHeader
{
public:
    friend ostream &operator<<(ostream &, const FileHeader&);
    friend class FileExec;
    FileHeader() {}
    FileHeader(const Elf32_Ehdr *ehdr) :
        type_(ehdr->e_type),
        machine_(ehdr->e_machine),
        version_(ehdr->e_version),
        entry_(ehdr->e_entry),
        phoff_(ehdr->e_phoff),
        shoff_(ehdr->e_shoff),
        flags_(ehdr->e_flags),
        ehsize_(ehdr->e_ehsize),
        phentsize_(ehdr->e_phentsize),
        phnum_(ehdr->e_phnum),
        shentsize_(ehdr->e_shentsize),
        shnum_(ehdr->e_shnum),
        shstrndx_(ehdr->e_shstrndx)
    {
        for (int i = 0; i < 16; i++) 
            ident_[i] = ehdr->e_ident[i];
    }

    UINT32 get_shoff()
    { return shoff_; }

    UINT16 get_shnum()
    { return shnum_; }

    UINT16 get_shstrndx()
    { return shstrndx_; }

private:
    UINT8 ident_[16];
    UINT16 type_;
    UINT16 machine_;
    UINT32 version_;
    UINT32 entry_;
    UINT32 phoff_;
    UINT32 shoff_;
    UINT32 flags_;
    UINT16 ehsize_;
    UINT16 phentsize_;
    UINT16 phnum_;
    UINT16 shentsize_;
    UINT16 shnum_;
    UINT16 shstrndx_;
};

class File
{
public:
    friend ostream &operator<<(ostream &, const File&);
    enum BINARY_TYPE {
        BINARY_CORE_TYPE,
        BINARY_SHARED_TYPE,
        BINARY_RELOCATABLE_TYPE,
        BINARY_EXECUTABLE_TYPE
    };

    File() {}
    File(const string &file_name, const BINARY_TYPE type);
    ~File();

    Elf32_Shdr *get_section_table() const 
    { return section_table_; }

    UINT8 *get_file_data() const 
    { return file_data_; }

    string get_file_name() const 
    { return file_name_; }

    UINT8 *get_section_strn_table() const
    { return section_strn_table_; }

    UINT16 get_section_number() const 
    { return section_number_; }
    
protected:
    shared_ptr<FileHeader> file_header_;
    BINARY_TYPE file_type_;
    UINT32 file_size_;
    UINT8 *file_data_;
    string file_name_;
    Elf32_Shdr *section_table_;
    UINT8 *section_strn_table_;
    UINT16 section_number_;

private:
    void _initPartFile();
    void _constructFileMeta();

    File(const File&);
    File &operator=(const File&);
};

class FileRel : public File
{
public:
    friend ostream &operator<<(ostream &, const FileRel&);
    FileRel() {}
    FileRel(const string&);

    Elf32_Shdr *get_relsym_table_dr() const 
    { return relsym_table_dr_; }

    Elf32_Sym *get_relsym_table() const 
    { return relsym_table_; }
    
    UINT8 *get_relsym_strn_table() const 
    { return relsym_strn_table_; }

    const vector<UINT32> &get_relocation_index() const 
    { return relocation_index_; }

private:
    Elf32_Shdr *relsym_table_dr_;
    UINT8 *relsym_strn_table_;
    Elf32_Sym *relsym_table_;
    vector<UINT32> relocation_index_;
};

class FileDyn : public File
{
public:
    FileDyn() {}
    FileDyn(const string&);

    Elf32_Shdr *get_dynsym_table_dr() const 
    { return dynsym_table_dr_; }

    UINT8 *get_dynsym_strn_table() const 
    { return dynsym_strn_table_; }

    Elf32_Sym *get_dynsym_table() const 
    { return dynsym_table_; }

private:
    Elf32_Shdr *dynsym_table_dr_;
    UINT8 *dynsym_strn_table_;
    Elf32_Sym *dynsym_table_;
};

class ProgramHeader 
{
public:
    friend ostream &operator<<(ostream &os, const ProgramHeader &phdr);
    friend class FileExec;
    ProgramHeader():
        data_(NULL), num_(0), size_(0) {}
    ProgramHeader(const SectionVec &);
    ~ProgramHeader();

private:
    UINT8 *data_;
    UINT32 num_;
    UINT32 size_;

    void _add_prog_phdr(Elf32_Phdr &phdr);
    void _add_prog_interp(Elf32_Phdr &phdr, const SectionVec &obj_sec_vec);
    void _add_prog_load(Elf32_Phdr &phdr, const SectionVec &obj_sec_vec);
    void _add_prog_dynamic(Elf32_Phdr &phdr, const SectionVec &obj_sec_vec);
    void _add_prog_note(Elf32_Phdr &phdr, const SectionVec &obj_sec_vec);
};

class SectionTable 
{
public:
    friend ostream &operator<<(ostream &os, const SectionTable &st);
    friend class FileExec;
    SectionTable():
        data_(NULL), num_(0), size_(0) {}
    SectionTable(const SectionVec &obj_sec_vec);
    ~SectionTable();
private:
    UINT8 *data_;
    UINT32 num_;
    UINT32 size_;
};

class FileExec : public File 
{
public:
    friend ostream &operator<<(ostream &os, const FileExec &f);
    FileExec() {}
    FileExec(const string &name):
        File(name, BINARY_EXECUTABLE_TYPE) {}
    void construct_section_table(const SectionVec&);
    void construct_program_header(const SectionVec&);
    void construct_file_header(const SectionVec&);
    void dump(const SectionVec&);
private:
    shared_ptr<SectionTable> sec_table_;
    shared_ptr<ProgramHeader> prog_header_;
};

#endif
