/*
 * =====================================================================================
 *
 *       Filename:  edge.cc
 *
 *    Description:  implementation of edge related classes
 *
 *        Version:  1.0
 *        Created:  01/13/2016 16:29:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wbn (), wangbn15@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "edge.h"

#include "block.h"

// ======= Edge =======
Edge::Edge()
{}

Edge::Edge(UINT32 id, shared_ptr<Block> from, shared_ptr<Block> to, ETYPE type):
    id_(id),
    from_(from),
    to_(to),
    type_(type)
{}

shared_ptr<Block> Edge::get_from_block() const 
{
    return from_;
}

shared_ptr<Block> Edge::get_to_block() const 
{
    return to_;
}

ETYPE Edge::get_type() const 
{
    return type_;
}

// ====== EdgeList ======
EdgeList* EdgeList::shared_edge_list_ = NULL;
EdgeList* EdgeList::sharedEdgeList()
{
    if (shared_edge_list_ == NULL)
        shared_edge_list_ = new EdgeList();
    return shared_edge_list_;
}

EdgeList::EdgeList()
{}

shared_ptr<Edge> EdgeList::get_edge(shared_ptr<Block> from, shared_ptr<Block> to, ETYPE type)
{
    shared_ptr<Edge> res;
    EdgeListT succ = from->get_succ_edges();
    for (EdgeIterT it = succ.begin(); it != succ.end(); it++) {
        if ((*it)->get_from_block() == from &&
                (*it)->get_to_block() == to &&
                (*it)->get_type() == type) {
            res = *it;
            break;
        }
    }
    return res;
}

bool EdgeList::edge_exist_or_not(shared_ptr<Block> from, shared_ptr<Block> to)
{
    EdgeListT succ = from->get_succ_edges();
    for (EdgeIterT it = succ.begin(); it != succ.end(); it++) {
        if ((*it)->get_from_block() == from &&
                (*it)->get_to_block() == to) {
            return true;
        }
    }
    return false;
}

bool EdgeList::edge_exist_or_not(shared_ptr<Block> from, shared_ptr<Block> to, ETYPE type)
{
    if (get_edge(from, to, type))
        return true;
    return false;
}

void EdgeList::add_bbl_edge(shared_ptr<Block> from, shared_ptr<Block> to, ETYPE type)
{
    if (edge_exist_or_not(from, to, type))
        return;
    shared_ptr<Edge> edge = make_shared<Edge>(edge_list_.size(), from, to, type);
    edge_list_.push_back(edge);

    from->add_succ_edge(edge);
    to->add_prev_edge(edge);
}
