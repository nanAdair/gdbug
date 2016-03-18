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
#include "function.h"
#include "instruction.h"
#include "log.h"
#include "disasm.h"
#include "upm.h"

#include <iostream>
#include <set>
#include <algorithm>
using namespace std;

shared_ptr<Block> get_reorder_target(const set<shared_ptr<Block> > &s);
void repair_block_content(shared_ptr<Block> source, shared_ptr<INSTRUCTION> new_first=0);
//void repair_block_content(shared_ptr<Block> source);

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

    // insert dump instr in the middle gadget
    shared_ptr<INSTRUCTION> first_instr = to_separate->get_first_instr();
    shared_ptr<INSTRUCTION> dump_instr = INSTRLIST->get_dump_instr();
    dump_instr->set_sec_type(first_instr->get_sec_type());

    INSTRLIST->add_instr_before(first_instr, dump_instr);
    to_separate->set_first_instr(dump_instr);

    // repair three blocks info
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
    //reorder_blocks.insert(to_separate);
    reorder_blocks.insert(to_insert);
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
    //int r;
    int r = rand() % s.size();
    //TODO: remove this
    //r = 10;
    //r = 5;
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

void repair_block_content(shared_ptr<Block> source, shared_ptr<INSTRUCTION> new_first)
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
    if (new_first) {
        new_first->set_block(source);
        new_first->set_sec_type(sec_type);
        source->set_first_instr(new_first);
        i++;
    }
    source->set_size(i);
}

void ROPObfuscation::make_gadgets(shared_ptr<Block> head, shared_ptr<Block> middle, PatchVec &upm_vec)
{
    _insert_instrs(head);
    _insert_instrs(middle);
    //_create_upm(head, upm_vec);
    //_create_upm(middle, upm_vec);
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
    INSTRLIST->add_instr_after(last_instr, ret);
    //INSTRLIST->add_instr_after(last_instr, push);
    //INSTRLIST->add_instr_after(push, ret);
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
        report(RL_FOUR, "get junk instr");
        shared_ptr<Block> to_insert = get_reorder_target(junk_insert_targets);
        shared_ptr<Block> to_insert_before = BLOCKLIST->get_next_block(to_insert);
        report(RL_FOUR, "insert junk bytes before block %d", to_insert_before->get_block_id());
        shared_ptr<INSTRUCTION> first_instr = to_insert_before->get_first_instr();
        shared_ptr<INSTRUCTION> dump_instr = INSTRLIST->get_dump_instr();
        dump_instr->set_sec_type(first_instr->get_sec_type());

        INSTRLIST->add_instr_before(first_instr, dump_instr);
        to_insert_before->set_first_instr(dump_instr);
    }
    report(RL_THREE, "insert junk instruction obfuscation end");
}

void StackObfuscation::obfuscate(PatchVec &upm_vec)
{
    report(RL_THREE, "move stack for some offset obfuscation begin");
    FunListT function_list = FUNLIST->get_function_list();    
    FunIterT it;
    for (it = function_list.begin(); it != function_list.end(); it++) {
        // manually set for debug, select insertion_sort function
        if ((*it)->get_function_id() == 11) {
            _add_stack_offset(*it);
            _amend_stack(*it);
            _insert_redundant_instrs(*it);
        }
    }
    report(RL_THREE, "move stack for some offset obfuscation end");
}

void StackObfuscation::_amend_stack(shared_ptr<Function> func)
{
    shared_ptr<INSTRUCTION> first_instr, last_instr;
    first_instr = func->get_first_block()->get_first_instr();
    last_instr = func->get_last_block()->get_last_instr();

    shared_ptr<INSTRUCTION> cur_instr = first_instr;
    while (cur_instr != last_instr) {
        //else 
            //cout << "false\t";
        if (cur_instr->addr_above_ebp_in_oprand()) {
            cout << "------------" << endl;
            cout << *cur_instr;
            shared_ptr<INSTRUCTION> to_add = _get_prepared_instr();
            int reg = _find_useful_reg(cur_instr); // find reg not used to add
            cout << "found useful reg is " << reg << endl;
            to_add->set_dest_operand(reg); // mov (%ebp), %eax, replace %eax using reg
            to_add->instruction2Binary(); // renew the binary
            cout << "before change " << *cur_instr;
            cur_instr->modify_ebp_operand(reg); // %ebp related instr add indirect offset
            cur_instr->instruction2Binary(); // renew the binary
            cout << "after change " << *cur_instr;
            INSTRLIST->add_instr_before(cur_instr, to_add);
            shared_ptr<Block> cur_block = cur_instr->get_block();
            if (cur_instr == cur_block->get_first_instr())
                repair_block_content(cur_block, to_add);
                //cur_block->set_first_instr(to_add);
            else
                repair_block_content(cur_block);
        }
        cur_instr = INSTRLIST->get_next_instr(cur_instr);
    }
}

void StackObfuscation::_insert_redundant_instrs(shared_ptr<Function> func)
{
    int number = 0;
    shared_ptr<INSTRUCTION> first_instr, last_instr;
    first_instr = func->get_first_block()->get_first_instr();
    last_instr = func->get_last_block()->get_last_instr();

    shared_ptr<INSTRUCTION> cur_instr = first_instr;
    while (cur_instr != last_instr) {
        number++;
        cur_instr = INSTRLIST->get_next_instr(cur_instr);
    }

    cout << dec << number << endl;
    InstrListT instrs = INSTRLIST->get_instr_list();
    InstrIterT head = instrs.begin();
    while (*head != first_instr) 
        head++;

    //cout << **head;
    int new_instrs_number = 5;
    int i = 0;
    while (i < new_instrs_number) {
        int r = rand() % number;
        InstrIterT it = head;
        advance(it, r);

        //TODO: add instr
        shared_ptr<INSTRUCTION> to_add = _get_redundant_instr(*it, 3);

        INSTRLIST->add_instr_before(*it, to_add);
        shared_ptr<Block> cur_block = (*it)->get_block();
        if ((*it) == cur_block->get_first_instr())
            repair_block_content(cur_block, to_add);
        else
            repair_block_content(cur_block);
        cout << "insertion instr choice " << endl;
        cout << **it;
        cout << *to_add;
        i++;
    }
}

shared_ptr<SCInstr> StackObfuscation::_get_redundant_instr(shared_ptr<SCInstr> cur_instr, int choice)
{
    unsigned char above_read_buffer[] = {0x8b, 0x45, 0x0}; // mov (%ebp), %eax
    unsigned char above_write_buffer[] = {0x89, 0x45, 0x0}; // mov %eax, (%ebp)
    unsigned char below_read_buffer[] = {0x8b, 0x45, 0x0}; // mov (%ebp), %eax

    Disasm disasm;
    INSTRUCTION *res_instr_raw = new SCInstr();
    switch (choice) {
        case 1:
            disasm.disassembler(reinterpret_cast<INT8*>(above_read_buffer), sizeof(above_read_buffer) / sizeof(unsigned char), 0, 0, res_instr_raw);
            break;
        case 2:
            disasm.disassembler(reinterpret_cast<INT8*>(above_write_buffer), sizeof(above_write_buffer) / sizeof(unsigned char), 0, 0, res_instr_raw);
            break;
        case 3:
            disasm.disassembler(reinterpret_cast<INT8*>(below_read_buffer), sizeof(below_read_buffer) / sizeof(unsigned char), 0, 0, res_instr_raw);
            break;
        default:
            report(RL_ONE, "choice of random insertion instruction is wrong");
            exit(0);
    }

    shared_ptr<SCInstr> res_instr(res_instr_raw); // mov (%ebp), %eax
    int reg = _find_useful_reg(cur_instr); // find reg not used to add
    cout << "found useful reg is " << reg << endl;
    int limit;
    int offset;
    if (choice == 1 || choice == 2) { // for above read and write generate [0x5, 0x1c]
        limit = 23;
        offset = 5;
    }
    else                            { // for below read generate [1, 0x2a]
        limit = 0x2a;
        offset = 1;
    }
    int displacement = rand() % limit + offset;
    if (choice == 3)
        displacement = 0 - displacement;
    cout << "displacement is " << displacement << endl;
    if (choice == 1 || choice == 3) {
        res_instr->set_dest_operand(reg);
        res_instr->set_src1_operand(EBP, displacement);
    }
    else {
        res_instr->set_src1_operand(reg);
        res_instr->set_dest_operand(EBP, displacement);
    }
    res_instr->instruction2Binary(); // renew the binary

    return res_instr;
}

shared_ptr<SCInstr> StackObfuscation::_get_prepared_instr()
{
    Disasm disasm;
    unsigned char mov_buffer[] = {0x8b, 0x45, 0x0}; // mov (%ebp), %eax
    INSTRUCTION *mov_instr = new SCInstr();
    int res = disasm.disassembler(reinterpret_cast<INT8*>(mov_buffer), sizeof(mov_buffer) / sizeof(unsigned char), 0, 0, mov_instr);
    shared_ptr<SCInstr> prepare_mov(mov_instr); // mov (%ebp), %eax

    return prepare_mov;
}

int StackObfuscation::_find_useful_reg(shared_ptr<SCInstr> instr)
{
    int res = -1;
    set<int> used;
    shared_ptr<SCInstr> cur_instr = instr;
    int i = 0;
    while (i < 10) {
        cur_instr->accumulate_used_reg(used);
        if (!cur_instr->isPCChangingClass() && !cur_instr->isPushClass() && !cur_instr->isPopClass()) {
            int dest_reg = cur_instr->get_reg_from_dest();
            //if (dest_reg == EBP || dest_reg == ESP)
                //continue;
            if (used.find(dest_reg) == used.end()) {
                res = dest_reg;
                break;
            }
        }
        if (cur_instr->isCallClass())
            return EAX;
        if (!cur_instr->isPCChangingClass()) 
            cur_instr = INSTRLIST->get_next_instr(cur_instr);
        else
            cur_instr = cur_instr->get_jump_target();
        i++;
        //cout << i << endl;
    }
    return res;
}

void StackObfuscation::_add_stack_offset(shared_ptr<Function> func)
{
    shared_ptr<Block> head_block;
    head_block = _find_head_block(func);
    if (!head_block)
        report(RL_ONE, "head block not found another situation for stack move");
    report(RL_FOUR, "find the target prologue block %d", head_block->get_block_id());
    _add_function_prologue(head_block);

    shared_ptr<Block> tail_block;
    tail_block = _find_tail_block(func);
    if (!tail_block)
        report(RL_ONE, "tail block not found another situation for stack move");
    report(RL_FOUR, "find the target epilogue block %d", tail_block->get_block_id());
    _add_function_ending(tail_block);

}

void StackObfuscation::_add_function_prologue(shared_ptr<Block> head_block)
{
    shared_ptr<INSTRUCTION> cur_instr, last_instr;
    cur_instr = head_block->get_first_instr();
    last_instr = head_block->get_last_instr();
    while (cur_instr != last_instr) {
        // find instruction 'push %ebp'
        if (cur_instr->get_opcode() == 0x55) 
            break;
        cur_instr = INSTRLIST->get_next_instr(cur_instr);
    }

    Disasm disasm;
    //TODO: change call_buffer
    unsigned char call_buffer[] = {0xb8, 0x20, 0, 0, 0}; // call random(), mov 0x20, %eax for now
    unsigned char sub_buffer[] = {0x2b, 0xe0};
    unsigned char mov_buffer1[] = {0x89, 0x04, 0x24};
    INSTRUCTION *call_instr = new INSTRUCTION();
    INT8 res = disasm.disassembler(reinterpret_cast<INT8*>(call_buffer), sizeof(call_buffer) / sizeof(unsigned char), 0, 0, call_instr);
    INSTRUCTION *sub_instr = new INSTRUCTION();
    res = disasm.disassembler(reinterpret_cast<INT8*>(sub_buffer), sizeof(sub_buffer) / sizeof(unsigned char), 0, 0, sub_instr);
    INSTRUCTION *mov_instr1 = new INSTRUCTION();
    res = disasm.disassembler(reinterpret_cast<INT8*>(mov_buffer1), sizeof(mov_buffer1) / sizeof(unsigned char), 0, 0, mov_instr1);

    shared_ptr<INSTRUCTION> call(call_instr); // mov 0x20, %eax
    shared_ptr<INSTRUCTION> sub(sub_instr);   // sub %eax, %esp
    shared_ptr<INSTRUCTION> mov1(mov_instr1); // mov %eax, (%esp)

    INSTRLIST->add_instr_after(cur_instr, call);
    INSTRLIST->add_instr_after(call, sub);
    INSTRLIST->add_instr_after(sub, mov1);
    
    repair_block_content(head_block);
}

void StackObfuscation::_add_function_ending(shared_ptr<Block> tail_block)
{
    shared_ptr<INSTRUCTION> cur_instr, first_instr;
    first_instr = INSTRLIST->get_prev_instr(tail_block->get_first_instr());
    cur_instr = tail_block->get_last_instr();
    while (cur_instr != first_instr) {
        // find instruction 'pop %ebp' or 'leave'
        if (cur_instr->get_opcode() == 0x5d || cur_instr->get_opcode() == 0xc9) 
            break;
        cur_instr = INSTRLIST->get_prev_instr(cur_instr);
    }
    cur_instr = INSTRLIST->get_prev_instr(cur_instr);

    //unsigned char add_buffer[] = {0x03, 0x6d, 0}; // add (%ebp), %ebp
    unsigned char mov_buffer[] = {0x8b, 0x55, 0}; // mov (%ebp), %edx
    //unsigned char add_buffer[] = {0x03, 0xea}; // add %ebp, %edx
    unsigned char add_buffer[] = {0x03, 0xe2}; // add %esp, %edx
    INSTRUCTION *mov_instr = new INSTRUCTION();
    INSTRUCTION *add_instr = new INSTRUCTION();

    Disasm disasm;
    INT8 res = disasm.disassembler(reinterpret_cast<INT8*>(mov_buffer), sizeof(mov_buffer) / sizeof(unsigned char), 0, 0, mov_instr);
    res = disasm.disassembler(reinterpret_cast<INT8*>(add_buffer), sizeof(add_buffer) / sizeof(unsigned char), 0, 0, add_instr);

    shared_ptr<INSTRUCTION> mov(mov_instr); // mov (%ebp), %edx
    shared_ptr<INSTRUCTION> add(add_instr); // add %esp, %edx


    INSTRLIST->add_instr_after(cur_instr, mov);
    INSTRLIST->add_instr_after(mov, add);
    repair_block_content(tail_block);
}

shared_ptr<Block> StackObfuscation::_find_head_block(shared_ptr<Function> func)
{
    shared_ptr<Block> cur_block, last_block, head_block;
    cur_block = func->get_first_block();
    last_block = BLOCKLIST->get_next_block(func->get_last_block());
    while (cur_block != last_block) {
        bool find_target = false;
        if (find_target)
            break;
        shared_ptr<INSTRUCTION> cur_instr, last_instr;
        cur_instr = cur_block->get_first_instr();
        last_instr = cur_block->get_last_instr();
        while (!find_target && cur_instr != last_instr) {
            // find instruction 'push %ebp'
            if (cur_instr->get_opcode() == 0x55) {
                find_target = true;
                head_block = cur_block;
                break;
            }
            cur_instr = INSTRLIST->get_next_instr(cur_instr);
        }
        cur_block = BLOCKLIST->get_next_block(cur_block);
    }
    return head_block;
}

shared_ptr<Block> StackObfuscation::_find_tail_block(shared_ptr<Function> func)
{
    shared_ptr<Block> cur_block, first_block, tail_block;
    cur_block = func->get_last_block();
    first_block = BLOCKLIST->get_prev_block(func->get_first_block());
    while (cur_block != first_block) {
        bool find_target = false;
        if (find_target)
            break;
        shared_ptr<INSTRUCTION> cur_instr, first_instr;
        first_instr = INSTRLIST->get_prev_instr(cur_block->get_first_instr());
        cur_instr = cur_block->get_last_instr();
        while (!find_target && cur_instr != first_instr) {
            // find instruction 'pop %ebp' or 'leave'
            if (cur_instr->get_opcode() == 0x5d || cur_instr->get_opcode() == 0xc9) {
                find_target = true;
                tail_block = cur_block;
                break;
            }
            cur_instr = INSTRLIST->get_prev_instr(cur_instr);
        }
        cur_block = BLOCKLIST->get_prev_block(cur_block);
    }
    return tail_block;
}
