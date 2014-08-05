/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Anton Dukeman
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file Action_Node.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * An Action_Node in Graphplan
 */

#include "graphplan/Action_Node.hpp"

#include <string>
#include <set>

using std::set;
using std::string;

graphplan::Action_Node::Action_Node(const Action& a,
  set<Proposition_Node*> pre, set<Proposition_Node*> add,
  set<Proposition_Node*> del, set<Action_Node*> mutex) :
  action_(a), preconditions_(pre), adds_(add), deletes_(del), mutex_(mutex)
{
}

void
graphplan::Action_Node::add_precondition(Proposition_Node* p)
{
  preconditions_.insert(p);
}

void
graphplan::Action_Node::add_add(Proposition_Node* p)
{
  adds_.insert(p);
}

void
graphplan::Action_Node::add_delete(Proposition_Node* p)
{
  deletes_.insert(p);
}

void
graphplan::Action_Node::add_mutex(Action_Node* a)
{
  mutex_.insert(a);
}

const graphplan::Action&
graphplan::Action_Node::get_action() const
{
  return action_;
}

const set<graphplan::Proposition_Node*>&
graphplan::Action_Node::get_preconditions() const
{
  return preconditions_;
}

const set<graphplan::Proposition_Node*>&
graphplan::Action_Node::get_adds() const
{
  return adds_;
}

const set<graphplan::Proposition_Node*>&
graphplan::Action_Node::get_deletes() const
{
  return deletes_;
}

const set<graphplan::Action_Node*>&
graphplan::Action_Node::get_mutex() const
{
  return mutex_;
}

string
graphplan::Action_Node::get_name() const
{
  return action_.get_name();
}

bool
graphplan::Action_Node::is_instance_of(const Action& a) const
{
  return action_ == a;
}
