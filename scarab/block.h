/*
 * =====================================================================================
 *
 *       Filename:  block.h
 *
 *    Description:  definition of bbl related class
 *
 *        Version:  1.0
 *        Created:  01/11/2016 10:56:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef GDBUG_SCARAB_BLOCK_H
#define GDBUG_SCARAB_BLOCK_H

#include "type.h"

#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <fstream>
using std::ostream;
#include <set>
using std::set;

class SCInstr;
typedef SCInstr INSTRUCTION;
class Edge;
class Function;
class SectionVec;

class Block 
{
public:
    friend ostream &operator<<(ostream &, const Block&);
    Block();
    Block(UINT32, shared_ptr<INSTRUCTION>, shared_ptr<INSTRUCTION>);
    ~Block();

    UINT32 get_block_id() const;
    UINT32 get_block_size() const;
    shared_ptr<INSTRUCTION> get_first_instr() const;
    shared_ptr<INSTRUCTION> get_last_instr() const;
    shared_ptr<Function> get_function() const;
    EdgeListT get_succ_edges() const;
    bool get_junk_flag() const;
    void set_first_instr(shared_ptr<INSTRUCTION>);
    void set_last_instr(shared_ptr<INSTRUCTION>);
    void set_function(shared_ptr<Function>);
    void set_type(BTYPE);
    void set_size(UINT32);
    void add_prev_edge(shared_ptr<Edge>);
    void add_succ_edge(shared_ptr<Edge>);
    void set_junk_flag(bool);

private:
    UINT32 id_;
    UINT16 type_;
    INT64 flags_;
    UINT32 size_;
    shared_ptr<INSTRUCTION> first_instr_;
    shared_ptr<INSTRUCTION> last_instr_;
    EdgeListT prev_;
    EdgeListT succ_;
    shared_ptr<Function> fun_;
    bool is_junk_target_;
};

class BlockList
{
public:
    friend ostream &operator<<(ostream &, const BlockList&);
    static BlockList* sharedBlockList();
    static BlockList* shared_bbl_;
    shared_ptr<Block> HELL;

    UINT32 get_block_list_size() const;
    BlockListT get_block_list() const;
    shared_ptr<Block> get_prev_block(shared_ptr<Block> b);
    shared_ptr<Block> get_next_block(shared_ptr<Block> b);

    void add_block(shared_ptr<Block>, shared_ptr<Block>);
    shared_ptr<Block> remove_block(shared_ptr<Block>);
    set<shared_ptr<Block> > order_insert_candidates();
    set<shared_ptr<Block> > junk_insert_candidates();
    
    void create_bbl();
    void mark_bbl();
    void divide_bbl_by_instr(shared_ptr<Block>, shared_ptr<INSTRUCTION>);
    void update_sections_data(SectionVec &);

private:
    BlockList();
    BlockListT bbl_;
};

#endif
