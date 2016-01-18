/*
 * =====================================================================================
 *
 *       Filename:  edge.h
 *
 *    Description:  classes of edge from block to block
 *
 *        Version:  1.0
 *        Created:  01/13/2016 16:25:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef GDBUG_SCARAB_EDGE_H
#define GDBUG_SCARAB_EDGE_H

#include "type.h"

#include <memory>
using std::shared_ptr;
using std::make_shared;

class Block;

class Edge
{
public:
    Edge();
    Edge(UINT32 id, shared_ptr<Block> from, shared_ptr<Block> to, ETYPE type);

    shared_ptr<Block> get_from_block() const;
    shared_ptr<Block> get_to_block() const;
    ETYPE get_type() const;
private:
    UINT32 id_;
    ETYPE type_;
    EFLAG flags_;
    shared_ptr<Block> from_;
    shared_ptr<Block> to_;
};

class EdgeList 
{
public:
    static EdgeList *sharedEdgeList();
    static EdgeList *shared_edge_list_;
    EdgeList();

    shared_ptr<Edge> get_edge(shared_ptr<Block>, ETYPE);
    shared_ptr<Edge> get_edge(shared_ptr<Block>, shared_ptr<Block>, ETYPE);
    void add_bbl_edge(shared_ptr<Block>, shared_ptr<Block>, ETYPE);
    bool edge_exist_or_not(shared_ptr<Edge>);
    bool edge_exist_or_not(shared_ptr<Block> from, shared_ptr<Block> to);
    bool edge_exist_or_not(shared_ptr<Block>, shared_ptr<Block>, ETYPE);

private:
    EdgeListT edge_list_;
};
#endif
