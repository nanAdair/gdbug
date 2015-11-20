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
using namespace std;

void binaryAbstraction(SectionVec &, int argc, char *argv[]);
int main(int argc, char *argv[])
{
    SectionVec obj_sec_vec;
    binaryAbstraction(obj_sec_vec, argc, argv);
}

void binaryAbstraction(SectionVec &obj_sec_vec, int argc, char *argv[])
{
    //TODO: parameters parser
    string objfile_name(argv[1]);
    FileRel objfile(objfile_name);

    obj_sec_vec.init(objfile);
    vector<shared_ptr<Section> > merged_sections = obj_sec_vec.merge_sections();
}
