/*
 * =====================================================================================
 *
 *       Filename:  function.h
 *
 *    Description:  classes of function
 *
 *        Version:  1.0
 *        Created:  01/12/2016 15:50:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef GDBUG_SCARAB_FUNCTION_H
#define GDBUG_SCARAB_FUNCTION_H

#include "type.h"
#include <string>
using std::string;
#include <fstream>
using std::ostream;

class SymbolVec;
class Block;

class Function : public std::enable_shared_from_this<Function>

{
public:
    friend ostream& operator<<(ostream&, const Function&);
    friend class FunctionList;
    Function();
    Function(UINT32, string&, shared_ptr<Block>, shared_ptr<Block>);
    //~Function();

    UINT32 get_function_id() const;
    shared_ptr<Block> get_first_block() const;
    shared_ptr<Block> get_last_block() const;
    shared_ptr<Block> get_entry_block() const;
    shared_ptr<Block> get_exit_block() const;
    void set_last_block(shared_ptr<Block>);

private:
    Function(const Function&);
    Function& operator=(const Function&);

    void _setup();

    UINT32 id_;
    UINT32 flags_;
    shared_ptr<Block> first_block_;
    shared_ptr<Block> last_block_;
    string name_;
    shared_ptr<Block> entry_;
    shared_ptr<Block> exit_;
};

class FunctionList
{
public:
    friend ostream& operator<<(ostream&, const FunctionList&);
    static FunctionList *shared_fun_list_;
    static FunctionList *sharedFunctionList();
    FunctionList();
    ~FunctionList();

    FunListT get_function_list() const;

    void add_function(shared_ptr<Function> fun);

    void mark_functions(const SymbolVec&);
    void create_function_list(const SymbolVec&);
    void resolve_entryless_function();

private:
    FunctionList(const FunctionList&);
    FunctionList& operator=(const FunctionList&);

    FunListT fun_list_;
};
#endif
