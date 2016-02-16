/*
 * =====================================================================================
 *
 *       Filename:  obfuscation.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/15/2016 11:00:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <memory>
using std::shared_ptr;

#include "type.h"

class Block;
class PatchVec;

class Obfuscation
{
public:
    virtual void obfuscate(PatchVec&) = 0;
    virtual ~Obfuscation(){}
};

class ROPObfuscation : public Obfuscation 
{
public:
    void obfuscate(PatchVec&);
private:
    shared_ptr<Block> random_block();
    shared_ptr<Block> cut_block(shared_ptr<Block>);
    void repair_block_content(shared_ptr<Block>);
    void _insert_instrs(shared_ptr<Block>);
    void _create_upm(shared_ptr<Block>, PatchVec&);
    void repair_block_relationship(shared_ptr<Block>);
    void make_gadgets(shared_ptr<Block>, shared_ptr<Block>, PatchVec&);
};
