/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  main routine
 *
 *        Version:  1.0
 *        Created:  11/16/2015 14:46:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string>
#include <iostream>

#include "file.h"
#include "section.h"
#include "symbol.h"
#include "relocation.h"
#include "version.h"
#include "instruction.h"
#include "upm.h"
#include "log.h"
using namespace std;

void binaryAbstraction(SectionVec&, SymbolVec&, RelocationVec&, int argc, char *argv[]);
void patchSectionContent(SectionVec&, const SymbolVec&, int argc, char *argv[]);
void writeOut(const SectionVec &obj_sec_vec, string name);
void finalizeLayout(SectionVec&, PatchVec&);

int main(int argc, char *argv[])
{
    SectionVec obj_sec_vec;
    SymbolVec obj_sym_vec;
    RelocationVec obj_rel_vec;
    binaryAbstraction(obj_sec_vec, obj_sym_vec, obj_rel_vec, argc, argv);
    
    INSTRLIST->disassemble2(obj_sec_vec);
    INSTRLIST->construct_cfg(obj_sym_vec);
    //cout << *INSTRLIST;
    //cout << obj_sec_vec;

    PatchVec upm_vec;
    obj_rel_vec.construct_upm(obj_sec_vec, upm_vec);

    finalizeLayout(obj_sec_vec, upm_vec);
    patchSectionContent(obj_sec_vec, obj_sym_vec, argc, argv);
    string res("output");
    writeOut(obj_sec_vec, res);
}

/*-----------------------------------------------------------------------------
 *  Construct all the basic info need for obfuscation
 *-----------------------------------------------------------------------------*/
void binaryAbstraction(SectionVec &obj_sec_vec, SymbolVec &obj_sym_vec, RelocationVec &obj_rel_vec, int argc, char *argv[])
{
    report(RL_THREE, "Binary Abstraction Begin");
    //TODO: parameters parser
    string objfile_name(argv[1]);
    FileRel objfile(objfile_name);

    /* Construct section, symbol, relocation list from obj file */
    obj_sec_vec.init(objfile);
    SectionVec merged_sections = obj_sec_vec.merge_sections();
    obj_sym_vec.init(objfile, obj_sec_vec, merged_sections);
    obj_rel_vec.init(objfile, obj_sec_vec, merged_sections, obj_sym_vec);

    /* Construct dynamic symbols list from .so files */
    SymbolDynVec dyn_sym_vec;
    // DEFAULT: the last parameter is ld path
    string ld_file(argv[argc-1]);
    vector<string> so_files;
    for (int i = 0; i < argc-3; i++) {
        string dynfile_name(argv[i+2]);
        report(RL_FOUR, "handle so file");
        so_files.push_back(dynfile_name.substr(dynfile_name.find_last_of("/")+1));

        FileDyn dynfile(dynfile_name);

        SectionVec so_sec_vec;
        so_sec_vec.init(dynfile);
        VersionVec ver_vec(so_sec_vec);
        SymbolDynVec cur_dynsym_vec(dynfile, so_sec_vec, ver_vec);

        dyn_sym_vec.add_from_SDVec(obj_sym_vec, cur_dynsym_vec);
    }

    obj_sec_vec.fill_sections_content(ld_file, so_files, dyn_sym_vec);
    obj_sec_vec.allocate_address();
    obj_sym_vec.update_symbols_value(obj_sec_vec);
    obj_rel_vec.apply_relocations(obj_sec_vec, dyn_sym_vec);

    report(RL_THREE, "Binary Abstraction Done");
}

void finalizeLayout(SectionVec &obj_sec_vec, PatchVec &upm_vec)
{
    int change = 0;
    do {
        report(RL_THREE, "finalize sections adddress");
        INSTRLIST->update_sections_size(obj_sec_vec);
        obj_sec_vec.allocate_address();
        INSTRLIST->update_instr_address(obj_sec_vec);

        change = INSTRLIST->update_pc_relative_jumps();
        change += upm_vec.apply();
        cout << "change: " << change << endl;
    } while (change);

    INSTRLIST->update_sections_data(obj_sec_vec);
}

/*-----------------------------------------------------------------------------
 *  fill other non-exec section content after obfuscation done
 *-----------------------------------------------------------------------------*/
void patchSectionContent(SectionVec &obj_sec_vec, const SymbolVec &obj_sym_vec, int argc, char *argv[])
{
    report(RL_THREE, "Patch Section Final Content Begin");
    vector<string> so_files;
    for (int i = 0; i < argc-3; i++) {
        string file_name = string(argv[i+2]);
        so_files.push_back(file_name.substr(file_name.find_last_of("/")+1));
    }

    obj_sec_vec.renew_sections_content(so_files, obj_sym_vec);
    report(RL_THREE, "Patch Section Final Content End");
}


/*-----------------------------------------------------------------------------
 *  construct header, program table, section table of file and write out
 *-----------------------------------------------------------------------------*/
void writeOut(const SectionVec &obj_sec_vec, string name)
{
    report(RL_THREE, "Start construct file header and tail");
    FileExec output(name);
    output.construct_section_table(obj_sec_vec);
    output.construct_program_header(obj_sec_vec);
    output.construct_file_header(obj_sec_vec);
    output.dump(obj_sec_vec);
    report(RL_THREE, "All done!");
}
