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

class SCInstr;
typedef SCInstr INSTRUCTION;
class Edge;
class Function;

class Block 
{
public:
    friend ostream &operator<<(ostream &, const Block&);
    Block();
    Block(UINT32, shared_ptr<INSTRUCTION>, shared_ptr<INSTRUCTION>);
    ~Block();

    shared_ptr<INSTRUCTION> get_first_instr() const;
    shared_ptr<INSTRUCTION> get_last_instr() const;
    void set_last_instr(shared_ptr<INSTRUCTION>);
    void set_function(shared_ptr<Function>);
    void set_type(BTYPE);

private:
    UINT32 id_;
    UINT16 type_;
    INT64 flags_;
    shared_ptr<INSTRUCTION> first_instr_;
    shared_ptr<INSTRUCTION> last_instr_;
    EdgeListT pred_;
    EdgeListT succ_;
    shared_ptr<Function> fun_;
};

class BlockList
{
public:
    friend ostream &operator<<(ostream &, const BlockList&);
    static BlockList* sharedBlockList();
    static BlockList* shared_bbl_;
    shared_ptr<Block> HELL;

    BlockListT get_block_list() const;
    
    void create_bbl();
    void mark_bbl();
    void divide_bbl_by_instr(shared_ptr<Block>, shared_ptr<INSTRUCTION>);

private:
    BlockList();
    BlockListT bbl_;
};

#endif
