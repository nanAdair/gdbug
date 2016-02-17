/*
 * =====================================================================================
 *
 *       Filename:  obfuscation.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/15/2016 14:50:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <ctime>

#include "obfuscation.h"
#include "block.h"
#include "instruction.h"
#include "log.h"
#include "disasm.h"
#include "upm.h"

#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

shared_ptr<Block> get_reorder_target(const set<shared_ptr<Block> > &s);

void ROPObfuscation::obfuscate(PatchVec &upm_vec)
{
    report(RL_THREE, "rop obfuscation begin");
    set<shared_ptr<Block> > reorder_blocks = BLOCKLIST->order_insert_candidates();
    shared_ptr<Block> to_cut = random_block();
    if (!to_cut)
        report(RL_ONE, "target block doesn't exist");
    shared_ptr<Block> to_separate = cut_block(to_cut);
    if (!to_separate)
        report(RL_ONE, "target separate block doesn't exist");
    //repair_block_relationship(to_cut);

    make_gadgets(to_cut, to_separate, upm_vec);
    shared_ptr<Block> head = to_cut;
    int i = 0;
    while (i < 3) {
        repair_block_content(head);
        head = BLOCKLIST->get_next_block(head);
        i++;
    }

    report(RL_FOUR, "to cut block %d", to_cut->get_block_id());
    report(RL_FOUR, "separated block %d", to_separate->get_block_id());

    bool to_cut_in_set = 0;
    if (reorder_blocks.find(to_cut) != reorder_blocks.end()) {
        to_cut_in_set = true;
        reorder_blocks.erase(to_cut);
    }
    shared_ptr<Block> to_insert = get_reorder_target(reorder_blocks);
    report(RL_FOUR, "to insert block %d", to_insert->get_block_id());

    BLOCKLIST->remove_block(to_separate);
    BLOCKLIST->add_block(to_insert, to_separate);
    reorder_blocks.insert(to_cut);
    reorder_blocks.insert(to_separate);
    to_cut->set_junk_flag(true);
    to_separate->set_junk_flag(true);
    if (to_cut_in_set) {
        shared_ptr<Block> new_tail = BLOCKLIST->get_next_block(to_cut);
        reorder_blocks.insert(new_tail);
        new_tail->set_junk_flag(true);
    }
    report(RL_THREE, "rop obfuscation end");
}

shared_ptr<Block> get_reorder_target(const set<shared_ptr<Block> > &s)
{
    int r = rand() % s.size();
    //TODO: remove this
    //r = 10;
    //r = 6;
    set<shared_ptr<Block> >::const_iterator it(s.begin());
    advance(it, r);

    return *it;
}

/*-----------------------------------------------------------------------------
 *  get block to cut based on random algorithm or user choice
 *-----------------------------------------------------------------------------*/
shared_ptr<Block> ROPObfuscation::random_block()
{
    //TODO: debug sort program
    UINT32 selected_id = 0x27;
    //TODO: debug hello world program
    //UINT32 selected_id = 0x21;
    BlockIterT it;
    BlockListT bbl = BLOCKLIST->get_block_list();
    shared_ptr<Block> res;
    for (it = bbl.begin(); it != bbl.end(); it++) {
        if ((*it)->get_block_id() == selected_id) {
            res = (*it);
            break;
        }
    }
    if (res)
        report(RL_FOUR, "get target block");
    return res;
}

/*-----------------------------------------------------------------------------
 *  cut the block to three
 *-----------------------------------------------------------------------------*/
shared_ptr<Block> ROPObfuscation::cut_block(shared_ptr<Block> source)
{
    shared_ptr<Block> head, middle, tail;
    int size = 0;
    // get random instruction except the first and last
    int sum = source->get_block_size() - 2;

    int one, two, rand_start, rand_end;
    srand(time(0));
    one = rand() % sum + 1; 
    two = rand() % sum + 1;
    while (one == two)
        two = rand() % sum + 1;
    rand_start = one < two ? one : two;
    rand_end = one > two ? one : two;

    //TODO: delete this, manually set for debug
    rand_start = 3;
    rand_end = 5;

    shared_ptr<INSTRUCTION> first_instr, last_instr;
    shared_ptr<INSTRUCTION> start = source->get_first_instr();
    shared_ptr<INSTRUCTION> end = source->get_last_instr();
    int i = 0;
    while (start != end) {
        if (i == rand_start)
            first_instr = start;
        if (i == rand_end)
            last_instr = start;
        start = INSTRLIST->get_next_instr(start);
        i++;
    }

    head = source;
    middle = make_shared<Block>(BLOCKLIST->get_block_list_size(), first_instr, last_instr);
    middle->set_size(rand_end - rand_start + 1);
    middle->set_function(head->get_function());

    BLOCKLIST->add_block(head, middle);

    tail = make_shared<Block>(BLOCKLIST->get_block_list_size(), INSTRLIST->get_next_instr(last_instr), head->get_last_instr());
    tail->set_size(source->get_block_size() - rand_end - 1);
    tail->set_function(head->get_function());
    BLOCKLIST->add_block(middle, tail);

    head->set_last_instr(INSTRLIST->get_prev_instr(first_instr));
    head->set_size(rand_start);

    return middle;
}

void ROPObfuscation::repair_block_content(shared_ptr<Block> source)
{
    shared_ptr<INSTRUCTION> start = source->get_first_instr();
    shared_ptr<INSTRUCTION> end = INSTRLIST->get_next_instr(source->get_last_instr());
    INT32 sec_type = start->get_sec_type();
    int i = 0;
    while (start != end) {
        start->set_block(source);
        start->set_sec_type(sec_type);
        start = INSTRLIST->get_next_instr(start);
        i++;
    }
    source->set_size(i);
}

void ROPObfuscation::make_gadgets(shared_ptr<Block> head, shared_ptr<Block> middle, PatchVec &upm_vec)
{
    _insert_instrs(head);
    _insert_instrs(middle);
    _create_upm(head, upm_vec);
    _create_upm(middle, upm_vec);
}

void ROPObfuscation::_insert_instrs(shared_ptr<Block> b)
{
    Disasm disasm;
    unsigned char push_buffer[] = {0x68, 0, 0, 0, 0,};
    unsigned char ret_buffer[] = {0xc3};
    INSTRUCTION *push_instr = new INSTRUCTION();
    INT8 res = disasm.disassembler(reinterpret_cast<INT8*>(push_buffer), 5, 0, 0, push_instr);
    INSTRUCTION *ret_instr = new INSTRUCTION();
    res = disasm.disassembler(reinterpret_cast<INT8*>(ret_buffer), 1, 0, 0, ret_instr);

    shared_ptr<INSTRUCTION> last_instr = b->get_last_instr();
    shared_ptr<INSTRUCTION> push(push_instr);
    shared_ptr<INSTRUCTION> ret(ret_instr);
    INSTRLIST->add_instr_after(last_instr, push);
    INSTRLIST->add_instr_after(push, ret);
    b->set_last_instr(ret);
}

void ROPObfuscation::_create_upm(shared_ptr<Block> b, PatchVec &upm_vec)
{
    shared_ptr<INSTRUCTION> start, end, push;
    start = INSTRLIST->get_prev_instr(b->get_first_instr());
    end = b->get_last_instr();
    while (end != start) {
        if (end->isPushClass())
            break;
        end = INSTRLIST->get_prev_instr(end);
    }
    push = end;
    shared_ptr<INSTRUCTION> target;
    shared_ptr<Block> next_block = BLOCKLIST->get_next_block(b);
    target = next_block->get_first_instr();

    shared_ptr<Patch> to_add = make_shared<PatchInstrtoInstr32>(target, 0, push, 1, 0);
    upm_vec.add_patch(to_add);
}

void ROPObfuscation::repair_block_relationship(shared_ptr<Block> source)
{
    shared_ptr<Block> head, middle, tail;
    head = source;
    middle = BLOCKLIST->get_next_block(head);
    tail = BLOCKLIST->get_next_block(middle);
}

void JunkObfuscation::obfuscate(PatchVec &upm_vec)
{
    report(RL_THREE, "insert junk instruction obfuscation begin");
    //int num = 10;
    int num = 1;
    set<shared_ptr<Block> > junk_insert_targets = BLOCKLIST->junk_insert_candidates();
    report(RL_FOUR, "insertion targets are %d ", junk_insert_targets.size());
    for (int i = 0; i < num; i++) {
        shared_ptr<INSTRUCTION> dump_instr = INSTRLIST->get_dump_instr();
        report(RL_FOUR, "get junk instr");
        shared_ptr<Block> to_insert = get_reorder_target(junk_insert_targets);
        report(RL_FOUR, "insert junk bytes after block %d", to_insert->get_block_id());
        shared_ptr<INSTRUCTION> last_instr = to_insert->get_last_instr();
        dump_instr->set_sec_type(last_instr->get_sec_type());

        INSTRLIST->add_instr_after(last_instr, dump_instr);
        to_insert = BLOCKLIST->get_next_block(to_insert);
        to_insert->set_first_instr(dump_instr);
    }
    report(RL_THREE, "insert junk instruction obfuscation end");
}
