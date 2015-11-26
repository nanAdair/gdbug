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
#include "log.h"
using namespace std;

void binaryAbstraction(SectionVec&, SymbolVec&, RelocationVec&, int argc, char *argv[]);
int main(int argc, char *argv[])
{
    SectionVec obj_sec_vec;
    SymbolVec obj_sym_vec;
    RelocationVec obj_rel_vec;
    binaryAbstraction(obj_sec_vec, obj_sym_vec, obj_rel_vec, argc, argv);
}

void binaryAbstraction(SectionVec &obj_sec_vec, SymbolVec &obj_sym_vec, RelocationVec &obj_rel_vec, int argc, char *argv[])
{
    //TODO: parameters parser
    string objfile_name(argv[1]);
    FileRel objfile(objfile_name);

    /* Construct section, symbol, relocation list from obj file */
    obj_sec_vec.init(objfile);
    SectionVec merged_sections = obj_sec_vec.merge_sections();
    obj_sym_vec.init(objfile, obj_sec_vec, merged_sections);
    obj_rel_vec.init(objfile, obj_sec_vec, merged_sections, obj_sym_vec);

    cout << obj_sec_vec << endl;
    /* Construct dynamic symbols list from .so files */
    SymbolDynVec dyn_sym_vec;
    // DEFAULT: the last parameter is ld path
    vector<string> so_files;
    string ld_file(argv[argc-1]);
    for (int i = 0; i < argc-3; i++) {
        string dynfile_name(argv[i+2]);
        report(RL_FOUR, "handle so file");
        so_files.push_back(dynfile_name);

        FileDyn dynfile(dynfile_name);

        SectionVec so_sec_vec;
        so_sec_vec.init(dynfile);
        VersionVec ver_vec(so_sec_vec);
        SymbolDynVec cur_dynsym_vec(dynfile, so_sec_vec, ver_vec);

        dyn_sym_vec.addFromSDVec(obj_sym_vec, cur_dynsym_vec);
    }

    obj_sec_vec.fill_sections_content(ld_file, so_files, dyn_sym_vec);
    obj_sec_vec.allocate_address();
    cout << obj_sec_vec;
    //cout << hex << endl;
    //cout << dyn_sym_vec << endl;
}
