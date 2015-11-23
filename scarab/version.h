/*
 * =====================================================================================
 *
 *       Filename:  version.h
 *
 *    Description:  versions related to dynamic symbol
 *
 *        Version:  1.0
 *        Created:  11/23/2015 09:55:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GDBUG_SCARAB_VERSION_H
#define GDBUG_SCARAB_VERSION_h

#include <elf.h>

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <iostream>
using std::ostream;

#include "type.h"

class Section;
class SectionVec;

class Version
{
public:
    friend class VersionVec;
    Version(int, shared_ptr<Section>, shared_ptr<Section>);

    int get_next_offset() const 
    { return content_->vd_next; }

private:
    Elf32_Verdef *content_;
    Elf32_Verdaux *aux_;
    string name_;
};

class VersionVec
{
public:
    VersionVec() {}
    VersionVec(const SectionVec&);

    string get_version_name(UINT16 num) const;
private:
    vector<shared_ptr<Version> > ver_vec_;
};
#endif
