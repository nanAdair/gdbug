/*
 * =====================================================================================
 *
 *       Filename:  upm.cc
 *
 *    Description:  class implementation of upm
 *
 *        Version:  1.0
 *        Created:  12/21/2015 16:11:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "upm.h"

#include <cstring>

#include "section.h"
#include "instruction.h"
#include "log.h"

using namespace std;

bool PatchInstrtoInstr32::apply()
{
    //cout << "1" << endl;
    int old_val, new_val;
    UINT8 *target_address = dest_instr->get_instruction_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_instr->get_instruction_address() + addend;
    if (old_val != new_val) {
    //cout << "1" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        memcpy(target_address, &new_val, sizeof(int));
        return true;
    }
    return false;
}

bool PatchInstrtoInstrPC32::apply()
{
    //cout << "2" << endl;
    int old_val, new_val;
    UINT8 *target_address = dest_instr->get_instruction_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_instr->get_instruction_address() + addend - (dest_instr->get_instruction_address() + dest_offset);
    if (old_val != new_val) {
    //cout << "2" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        memcpy(target_address, &new_val, sizeof(int));
        return true;
    }
    return false;
}

bool PatchSectiontoInstr32::apply()
{
    int old_val, new_val;
    UINT8 *target_address = dest_instr->get_instruction_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_sec->get_section_address() + src_offset + addend;
    if (old_val != new_val) {
    //cout << "3" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        memcpy(target_address, &new_val, sizeof(int));
        return true;
    }
    return false;
}

bool PatchSectiontoInstrPC32::apply()
{
    //cout << "4" << endl;
    int old_val, new_val;
    UINT8 *target_address = dest_instr->get_instruction_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_sec->get_section_address() + src_offset + addend - (dest_instr->get_instruction_address() + dest_offset);
    if (old_val != new_val) {
    //cout << "4" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        memcpy(target_address, &new_val, sizeof(int));
        return true;
    }
    return false;
}

bool PatchSecSectoInstr32::apply()
{
    int old_val, new_val;
    UINT8 *target_address = dest_instr->get_instruction_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_sec->get_section_address() + src_offset - base_sec->get_section_address() + addend;
    //cout << "test ";
    //cout << *dest_instr;
    //cout << old_val << " " << new_val << endl;
    if (old_val != new_val) {
    //cout << "5" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        memcpy(target_address, &new_val, sizeof(int));
        return true;
    }
    return false;
}

bool PatchSectiontoSection32::apply()
{
    //cout << "5" << endl;
    int old_val, new_val;
    UINT8 *target_address = dest_sec->get_section_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_sec->get_section_address() + src_offset + addend;
    if (old_val != new_val) {
    //cout << "6" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        dest_sec->update_section_data(dest_offset, reinterpret_cast<UINT8*>(&new_val), sizeof(int));
        return true;
    }
    return false;
}

bool PatchSectiontoSectionPC32::apply()
{
    //cout << "6" << endl;
    int old_val, new_val;
    UINT8 *target_address = dest_sec->get_section_data() + dest_offset;
    old_val = *(reinterpret_cast<int*>(target_address));

    new_val = src_sec->get_section_address() + src_offset + addend - (dest_sec->get_section_address() + dest_offset);
    if (old_val != new_val) {
    //cout << "7" << endl;
        //cout << *dest_instr;
        //cout << old_val << " " << new_val << endl;
        dest_sec->update_section_data(dest_offset, reinterpret_cast<UINT8*>(&new_val), sizeof(int));
        return true;
    }
    return false;
}

int PatchVec::apply()
{
    report(RL_FOUR, "UPM apply");
    int change = 0;
    for (int i = 0; i < upm_vec.size(); i++) {
        if (upm_vec[i]->apply())
            change++;
    }
    return change;
}
