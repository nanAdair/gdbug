/*
 * =====================================================================================
 *
 *       Filename:  block.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/11/2016 14:38:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "block.h"

#include <string>
#include <iostream>
using namespace std;

#include "instruction.h"
#include "log.h"
#include "function.h"
#include "edge.h"

Block::Block():
    id_(0),
    type_(BT_INVALID),
    flags_(0)
{}

Block::Block(UINT32 i, shared_ptr<INSTRUCTION> one, shared_ptr<INSTRUCTION> two):
    Block()
{
    id_ = i;
    first_instr_ = one;
    last_instr_ = two;
}

Block::~Block()
{}

shared_ptr<INSTRUCTION> Block::get_first_instr() const 
{
    return first_instr_;
}

shared_ptr<INSTRUCTION> Block::get_last_instr() const 
{
    return last_instr_;
}

shared_ptr<Function> Block::get_function() const 
{
    return fun_;
}

EdgeListT Block::get_succ_edges() const 
{
    return succ_;
}

void Block::set_last_instr(shared_ptr<INSTRUCTION> instr)
{
    last_instr_ = instr;
}

void Block::set_function(shared_ptr<Function> fun)
{
    fun_ = fun;
}

void Block::set_type(BTYPE type)
{
    type_ = type;
}

void Block::add_prev_edge(shared_ptr<Edge> e)
{
    prev_.push_back(e);
}

void Block::add_succ_edge(shared_ptr<Edge> e)
{
    succ_.push_back(e);
}

// ===== BlockList ======
BlockList* BlockList::shared_bbl_ = NULL;

BlockList::BlockList()
{
    HELL = make_shared<Block>();
    HELL->set_type(BT_HELL);
    string fun_name("HELL");
    shared_ptr<Function> hell_fun = make_shared<Function>(0, fun_name, HELL, HELL);
    HELL->set_function(hell_fun);
    bbl_.push_back(HELL);
    FUNLIST->add_function(hell_fun);
}


BlockList* BlockList::sharedBlockList()
{
    if (shared_bbl_ == NULL) 
        shared_bbl_ = new BlockList();
    return shared_bbl_;
}

BlockListT BlockList::get_block_list() const
{
    return bbl_;
}

shared_ptr<Block> BlockList::get_prev_block(shared_ptr<Block> b)
{
    shared_ptr<Block> res;
    BlockIterT it; 
    for (it = bbl_.begin(); it != bbl_.end(); it++) {
        if (*it == b)
            break;
    }
    if (it == bbl_.begin() || it == bbl_.end())
        return res;
    res = *(--it);
    return res;
}

shared_ptr<Block> BlockList::get_next_block(shared_ptr<Block> b)
{
    shared_ptr<Block> res;
    BlockIterT it; 
    for (it = bbl_.begin(); it != bbl_.end(); it++) {
        if (*it == b)
            break;
    }
    if (it == bbl_.begin() || it == bbl_.end())
        return res;
    res = *it;
    return res;
}

void BlockList::mark_bbl()
{
    report(RL_FOUR, "mark instruction for bbl");
    InstrListT instrs = INSTRLIST->get_instr_list();
    shared_ptr<INSTRUCTION> first_instr = instrs.front();
    first_instr->set_flag(BBL_START);

    InstrIterT instrIter, nextInstrIter;
    for (instrIter = instrs.begin(); instrIter != instrs.end(); instrIter++) {
        if ((*instrIter)->isPCChangingClass() || (*instrIter)->isDataInstruction()) {
            (*instrIter)->set_flag(BBL_END);
            nextInstrIter = instrIter;
            nextInstrIter++;
            if (nextInstrIter != instrs.end()) {
                (*nextInstrIter)->set_flag(BBL_START);
            }
        }
        nextInstrIter = instrIter;
        nextInstrIter++;
        if (nextInstrIter == instrs.end()) {
            (*instrIter)->set_flag(BBL_END);
            (*instrIter)->set_flag(FUN_END);
            break;
        }
    }
}

void BlockList::create_bbl()
{
    report(RL_FOUR, "create basic block list");
    InstrListT instrs = INSTRLIST->get_instr_list();
    InstrIterT instrIter;
    shared_ptr<Block> new_bb;
    for (instrIter = instrs.begin(); instrIter != instrs.end(); instrIter++) {
        if ((*instrIter)->has_flag(BBL_START)) {
            new_bb = make_shared<Block>(static_cast<UINT32>(bbl_.size()), *instrIter, *instrIter);
            bbl_.push_back(new_bb);
            //cout << "[start]" << "\t" << **instrIter;
        }
        if ((*instrIter)->has_flag(BBL_END)) {
            new_bb->set_last_instr(*instrIter);
            //cout << "[end  ]" << "\t" << **instrIter;
        }
        (*instrIter)->set_block(new_bb);
    }
}

// ===== helper functions=======
ostream &operator<<(ostream &os, const Block &b)
{
    os << b.id_ << endl;
    os << *(b.first_instr_);
    os << *(b.last_instr_);
    return os;
}

ostream &operator<<(ostream &os, const BlockList &b)
{
    BlockIterT it;
    BlockListT bbl = b.get_block_list();
    shared_ptr<Function> fun;
    os << bbl.size() << endl;
    for (it = bbl.begin(); it != bbl.end(); it++) {
        // skip HELL block
        if (it == bbl.begin())
            continue;
        os << **it;
    }
}
