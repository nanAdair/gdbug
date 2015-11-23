/*
 * =====================================================================================
 *
 *       Filename:  version.cc
 *
 *    Description:  implementation of version classes
 *
 *        Version:  1.0
 *        Created:  11/23/2015 10:03:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "version.h"

#include "section.h"
#include "log.h"

Version::Version(int offset, shared_ptr<Section> dynstr, shared_ptr<Section> version_define)
{
    content_ = reinterpret_cast<Elf32_Verdef*> (version_define->get_section_data() + offset);
    int offset_aux = content_->vd_aux;
    aux_ = reinterpret_cast<Elf32_Verdaux*> (version_define->get_section_data() + offset + offset_aux);
    name_ = string(reinterpret_cast<char*> (dynstr->get_section_data() + aux_->vda_name));
}

VersionVec::VersionVec(const SectionVec &dsl)
{
    shared_ptr<Section> dynstr = dsl.get_section_by_name(".dynstr");
    shared_ptr<Section> version_define = dsl.get_section_by_name(".gnu.version_d");
    shared_ptr<Section> dynamic = dsl.get_section_by_name(".dynamic");

    int offset, number_of_versions;
    offset = 0;
    number_of_versions = std::dynamic_pointer_cast<SectionDynamic>(dynamic)->get_dynamic_attribute(DT_VERDEFNUM);

    for (int i = 0; i < number_of_versions; i++) {
        shared_ptr<Version> ver = make_shared<Version> (offset, dynstr, version_define);

        offset += ver->get_next_offset();
        ver_vec_.push_back(ver);
    }
    report(RL_FOUR, "complete version vector construction");
}

string VersionVec::get_version_name(UINT16 num) const 
{
    vector<shared_ptr<Version> >::const_iterator it;
    for (it = ver_vec_.begin(); it != ver_vec_.end(); it++) {
        if ((*it)->content_->vd_ndx == num)
            return (*it)->name_;
    }
    return string("");
}
