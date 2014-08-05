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
 * @file Proposition_Node.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * A Proposition_Node in Graphplan
 */

#include "graphplan/Proposition_Node.hpp"

#include <set>

using std::string;
using std::set;

graphplan::Proposition_Node::Proposition_Node(const Proposition& p,
  const bool d) :
  proposition_(p), deleted_(d)
{
}

graphplan::Proposition_Node::Proposition_Node(const Proposition_Node& p) :
  proposition_(p.proposition_), deleted_(p.deleted_)
{
}

void
graphplan::Proposition_Node::add_supply(Action_Node* a)
{
  supply_.insert(a);
}

void
graphplan::Proposition_Node::add_cause(Action_Node* a)
{
  causes_.insert(a);
}

void
graphplan::Proposition_Node::add_mutex(Proposition_Node* p)
{
  mutex_.insert(p);
}

string
graphplan::Proposition_Node::get_name() const
{
  return proposition_.get_name();
}

const graphplan::Proposition&
graphplan::Proposition_Node::get_proposition() const
{
  return proposition_;
}

const set<graphplan::Action_Node*>&
graphplan::Proposition_Node::get_supply() const
{
  return supply_;
}

const set<graphplan::Action_Node*>&
graphplan::Proposition_Node::get_cause() const
{
  return causes_;
}

const set<graphplan::Proposition_Node*>&
graphplan::Proposition_Node::get_mutex() const
{
  return mutex_;
}

bool
graphplan::Proposition_Node::is_deleted() const
{
  return deleted_;
}

bool
graphplan::Proposition_Node::instance_of(const Proposition& p) const
{
  return proposition_ == p;
}
