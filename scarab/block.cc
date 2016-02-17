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
#include "section.h"

Block::Block():
    id_(0),
    type_(BT_INVALID),
    flags_(0),
    is_junk_target_(false)
{}

Block::Block(UINT32 i, shared_ptr<INSTRUCTION> one, shared_ptr<INSTRUCTION> two):
    Block()
{
    id_ = i;
    first_instr_ = one;
    last_instr_ = two;
    // only insert junk instruction in .text and after jmp
    //if (last_instr_->get_sec_type() == SECTION_TEXT && last_instr_->isJmpClass())
        //is_junk_target_ = true;
}

Block::~Block()
{}

UINT32 Block::get_block_id() const 
{
    return id_;
}

UINT32 Block::get_block_size() const 
{
    return size_;
}

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

bool Block::get_junk_flag() const 
{
    return is_junk_target_;
}

void Block::set_first_instr(shared_ptr<INSTRUCTION> instr)
{
    first_instr_ = instr;
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

void Block::set_size(UINT32 size)
{
    size_ = size;
}

void Block::set_junk_flag(bool b)
{
    is_junk_target_ = b;
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

UINT32 BlockList::get_block_list_size() const 
{
    return bbl_.size();
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
    if (it == bbl_.end() || ++it == bbl_.end())
        return res;
    res = *(it);
    return res;
}

void BlockList::add_block(shared_ptr<Block> source, shared_ptr<Block> to_add)
{
    BlockIterT it;
    for (it = bbl_.begin(); it != bbl_.end(); it++) 
        if (*it == source)
            break;
    bbl_.insert(++it, to_add);
}

shared_ptr<Block> BlockList::remove_block(shared_ptr<Block> b)
{
    BlockIterT it;
    for (it = bbl_.begin(); it != bbl_.end(); it++) {
        if (*it == b)
            break;
    }
    bbl_.erase(it);
    return b;
}

/*-----------------------------------------------------------------------------
 *  candidates to insert rop gadgets
 *  .text && jmp 
 *-----------------------------------------------------------------------------*/
set<shared_ptr<Block> > BlockList::order_insert_candidates()
{
    BlockIterT it;
    set<shared_ptr<Block> > res;
    for (it = bbl_.begin(); it != bbl_.end(); it++) {
        if (it == bbl_.begin())
            continue;
        shared_ptr<INSTRUCTION> last_instr = (*it)->get_last_instr();
        if (last_instr->get_sec_type() != SECTION_TEXT)
            continue;
        if (last_instr->isJmpClass())
        //if (last_instr->isJmpClass() || last_instr->isReturnClass())
            res.insert(*it);
    }
    return res;
}

set<shared_ptr<Block> > BlockList::junk_insert_candidates()
{
    BlockIterT it;
    set<shared_ptr<Block> > res;
    for (it = bbl_.begin(); it != bbl_.end(); it++) {
        if (it == bbl_.begin())
            continue;
        if ((*it)->get_junk_flag()) {
            res.insert(*it);
            continue;
        }
        shared_ptr<INSTRUCTION> last_instr = (*it)->get_last_instr();
        if (last_instr->get_sec_type() == SECTION_TEXT && last_instr->isJmpClass())
            res.insert(*it);
    }
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
    UINT32 num = 0;
    for (instrIter = instrs.begin(); instrIter != instrs.end(); instrIter++) {
        num++;
        if ((*instrIter)->has_flag(BBL_START)) {
            new_bb = make_shared<Block>(static_cast<UINT32>(bbl_.size()), *instrIter, *instrIter);
            bbl_.push_back(new_bb);
            //cout << "[start]" << "\t" << **instrIter;
        }
        if ((*instrIter)->has_flag(BBL_END)) {
            new_bb->set_last_instr(*instrIter);
            new_bb->set_size(num);
            num = 0;
            //cout << "[end  ]" << "\t" << **instrIter;
        }
        (*instrIter)->set_block(new_bb);
    }
}

void BlockList::update_sections_data(SectionVec &obj_sec_vec)
{
    report(RL_FOUR, "update sections data");
    BlockIterT it; 
    int offset = 0;
    int last_sec, cur_sec;
    last_sec = -1;
    shared_ptr<Section> sec;

    for(it = bbl_.begin(); it != bbl_.end(); it++) {
        if (it == bbl_.begin())
            continue;
        //cout << (*it);
        shared_ptr<INSTRUCTION> cur_instr = (*it)->get_first_instr();
        shared_ptr<INSTRUCTION> last_instr = (*it)->get_last_instr();
        cur_sec = cur_instr->get_sec_type();
        if (cur_sec != last_sec) {
            switch(cur_sec) {
                case SECTION_INIT:
                    sec = obj_sec_vec.get_section_by_name(INIT_SECTION_NAME);
                    break;
                case SECTION_TEXT:
                    sec = obj_sec_vec.get_section_by_name(TEXT_SECTION_NAME);
                    break;
                case SECTION_FINI:
                    sec = obj_sec_vec.get_section_by_name(FINI_SECTION_NAME);
                    break;
                case SECTION_PLT:
                    sec = obj_sec_vec.get_section_by_name(PLT_SECTION_NAME);
                    break;
                default:
                    report(RL_ONE, "can't handle instr sec type for now");
                    exit(0);
            }

            last_sec = cur_sec;
            sec->clear_section_data();
        }

        while (cur_instr != last_instr) {
            sec->expand_section_data(cur_instr->get_instruction_data(), cur_instr->get_instruction_size(), 1);
            cur_instr = INSTRLIST->get_next_instr(cur_instr);
        }
        sec->expand_section_data(cur_instr->get_instruction_data(), cur_instr->get_instruction_size(), 1);
    }
}
// ===== helper functions=======
ostream &operator<<(ostream &os, const Block &b)
{
    os << dec << b.id_ << endl;
    os << b.size_ << endl;
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
    return os;
}
