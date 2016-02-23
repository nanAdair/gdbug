/*
 * =====================================================================================
 *
 *       Filename:  function.cc
 *
 *    Description:  implementation of function
 *
 *        Version:  1.0
 *        Created:  01/12/2016 15:59:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "function.h"

#include <iostream>
using namespace std;

#include "instruction.h"
#include "block.h"
#include "symbol.h"
#include "log.h"
#include "edge.h"

// ====== Function ===== 
Function::Function():
    id_(0),
    flags_(0)
{
}

void Function::_setup()
{
    entry_ = make_shared<Block>();
    exit_ = make_shared<Block>();
    entry_->set_type(BT_ENTRY);
    exit_->set_type(BT_EXIT);
    entry_->set_function(shared_from_this());
    exit_->set_function(shared_from_this());
}

Function::Function(UINT32 i, string &s, shared_ptr<Block> one, shared_ptr<Block> two):
    Function()
{
    id_ = i;
    name_ = s;
    first_block_ = one;
    last_block_ = two;
}

UINT32 Function::get_function_id() const 
{
    return id_;
}

void Function::set_last_block(shared_ptr<Block> block)
{
    last_block_ = block;
}

shared_ptr<Block> Function::get_first_block() const
{
    return first_block_;
}

shared_ptr<Block> Function::get_last_block() const
{
    return last_block_;
}

shared_ptr<Block> Function::get_entry_block() const
{
    return entry_;
}


shared_ptr<Block> Function::get_exit_block() const
{
    return exit_;
}

// ==== FunctionList =====
FunctionList::FunctionList()
{}

FunctionList* FunctionList::shared_fun_list_ = NULL;

FunctionList* FunctionList::sharedFunctionList()
{
    if (shared_fun_list_ == NULL)
        shared_fun_list_ = new FunctionList();
    return shared_fun_list_;
}

void FunctionList::add_function(shared_ptr<Function> fun)
{
    fun_list_.push_back(fun);
}

FunListT FunctionList::get_function_list() const 
{
    return fun_list_;
}

void FunctionList::mark_functions(const SymbolVec &osl)
{
    report(RL_FOUR, "mark functions");
    UINT32 number = osl.get_sym_vec_size();
    for (UINT32 i = 0; i < number; i++) {
        shared_ptr<Symbol> cur_sym = osl.get_symbol_by_index(i);
        if (cur_sym->get_symbol_type() != STT_FUNC)
            continue;
        shared_ptr<INSTRUCTION> instr = INSTRLIST->get_instr_by_exact_address(cur_sym->get_symbol_value());
        if (!instr)
            continue;
        instr->set_flag(FUN_START);
        instr->set_flag(BBL_START);

        if (instr = INSTRLIST->get_prev_instr(instr)) {
            instr->set_flag(FUN_END);
            instr->set_flag(BBL_END);
        }
    }
}

void FunctionList::create_function_list(const SymbolVec &osl)
{
    report(RL_FOUR, "create function list");
    BlockIterT it;
    BlockListT bbl = BLOCKLIST->get_block_list();
    shared_ptr<Function> fun;
    for (it = bbl.begin(); it != bbl.end(); it++) {
        if (it == bbl.begin())
            continue;
        shared_ptr<INSTRUCTION> first_instr = (*it)->get_first_instr();
        shared_ptr<INSTRUCTION> last_instr = (*it)->get_last_instr();
        //cout << **it;
        if (first_instr->has_flag(FUN_START)) {
            string function_name = osl.get_symname_by_addr(first_instr->get_instruction_address());
            fun = make_shared<Function>(fun_list_.size(), function_name, *it, *it);
            fun->_setup();
            fun_list_.push_back(fun);
        }
        else if (last_instr->has_flag(FUN_END)) 
            fun->set_last_block(*it);
        (*it)->set_function(fun);
    }
}

void FunctionList::resolve_entryless_function()
{
    report(RL_FOUR, "resolve entryless functions");
    for (FunIterT it = fun_list_.begin(); it != fun_list_.end(); it++) {
        if (it == fun_list_.begin())
            continue;
        if ((*it)->get_entry_block()->get_succ_edges().size() == 0) {
            EDGELIST->add_bbl_edge((*it)->get_entry_block(), (*it)->get_first_block(), ET_ENTRY);
        }
    }
}

// ===== helper functions=====
ostream &operator<<(ostream &os, const Function &f)
{
    os << dec << f.id_ << "\t" << f.name_ << endl;
    if (f.name_ == "HELL")
        return os;
    os << *(f.first_block_->get_first_instr());
    os << *(f.last_block_->get_last_instr());
    return os;
}

ostream &operator<<(ostream &os, const FunctionList &fl)
{
    list<shared_ptr<Function> >::const_iterator it;
    for (it = (fl.fun_list_).begin(); it != (fl.fun_list_).end(); it++)
        os << **it;
    return os;
}
