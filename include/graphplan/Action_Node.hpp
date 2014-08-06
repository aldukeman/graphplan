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
 * @file Action_Node.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * An Action_Node in Graphplan
 */

#ifndef _GRAPHPLAN_ACTION_NODE_HPP_
#define _GRAPHPLAN_ACTION_NODE_HPP_

#include <set>

#include "graphplan/Action.hpp"

namespace graphplan
{
  class Proposition_Node;

  class Action_Node
  {
  public:
    /// Constructor
    Action_Node(const Action& n);

    /// get base action
    const Action& get_action() const;

    /// get precondition list
    void add_precondition(const Proposition_Node* p);

    /// get add list
    void add_effect(Proposition_Node* p);

    /// get mutex list
    void add_mutex(Action_Node* a);

    /// get precondition list
    const std::set<const Proposition_Node*>& get_preconditions() const;

    /// get effects
    const std::set<Proposition_Node*>& get_effects() const;

    /// get mutex list
    const std::set<Action_Node*>& get_mutex() const;

    /// get name of action
    std::string get_name() const;

    /// instance of Action
    bool is_instance_of (const Action& a) const;

  protected:
    /// name of the proposition
    const Action action_;

    /// preconditions
    std::set<const Proposition_Node*> preconditions_;

    /// results
    std::set<Proposition_Node*> effects_;

    /// mutex
    std::set<Action_Node*> mutex_;
  }; // class Action_Node
} // namespace graphplan

#endif // _GRAPHPLAN_ACTION_NODE_HPP_
