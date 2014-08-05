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
 * @file Proposition_Node.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * A Proposition_Node in Graphplan
 */

#ifndef _GRAPHPLAN_PROPOSITION_NODE_H_
#define _GRAPHPLAN_PROPOSITION_NODE_H_

#include <string>
#include <set>

#include "graphplan/Proposition.hpp"

namespace graphplan
{
  class Action_Node;

  class Proposition_Node
  {
  public:
    /// Create instance of proposition
    Proposition_Node(const Proposition& p, const bool d = false);

    /// Copy Constructor
    Proposition_Node(const Proposition_Node& p);

    /// add another supply action node
    void add_supply(Action_Node* a);

    /// add another cause action node
    void add_cause(Action_Node* a);

    /// add another mutex node
    void add_mutex(Proposition_Node* p);

    /// get proposition name
    std::string get_name() const;

    /// get proposition
    const Proposition& get_proposition() const;

    /// get action nodes this is a precondition of
    const std::set<Action_Node*>& get_supply() const;

    /// get action nodes this is an effect of
    const std::set<Action_Node*>& get_cause() const;

    /// get proposition nodes this node is mutex with
    const std::set<Proposition_Node*>& get_mutex() const;

    /// get deleted
    bool is_deleted() const;

    /// determine if this is a proposition
    bool instance_of(const Proposition& p) const;

  protected:
    /// name of the proposition
    Proposition proposition_;

    /// deleted?
    bool deleted_;

    /// action nodes this proposition is a precondition of
    std::set<Action_Node*> supply_;

    /// action nodes this propositions is an effect of
    std::set<Action_Node*> causes_;

    /// mutex propositions
    std::set<Proposition_Node*> mutex_;
  }; // class Proposition_Node
} // namespace graphplan

#endif // _GRAPHPLAN_PROPOSITION_NODE_H_
