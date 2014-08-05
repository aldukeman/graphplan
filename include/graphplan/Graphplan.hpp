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
 * @file Graphplan.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * Graphplan header class
 */

#ifndef _GRAPHPLAN_GRAPHPLAN_H_
#define _GRAPHPLAN_GRAPHPLAN_H_

#include <vector>
#include <set>

#include "graphplan/Proposition.hpp"
#include "graphplan/Proposition_Node.hpp"
#include "graphplan/Action.hpp"
#include "graphplan/Action_Node.hpp"

namespace graphplan
{
  class Graphplan
  {
  public:
    /// Constructor
    Graphplan();

    /// Destructor
    ~Graphplan();

    /// add starting propositions
    void add_starting(const Proposition& p);

    /// add goal propositions
    void add_goal(const Proposition& p);

    /// add possible action
    void add_action(const Action& a);

    /// get starting propositions
    const std::set<Proposition>& get_starting() const;

    /// get available actions
    const std::set<Action>& get_actions() const;

    /// get goals
    const std::set<Proposition>& get_goals() const;

    /// find plan
    unsigned int plan(unsigned int iterations = 5);

    /// get string representation
    std::string to_string() const;

    /// check for goal state
    bool goal_check(const std::set<Proposition_Node*>& props) const;

  //protected:
    /// perform an action step
    void iteration(const std::set<Proposition_Node*>& props, 
      std::set<Proposition_Node*>& new_props, 
      std::set<Action_Node*>& new_actions);

    /// connect precondition nodes
    void connect_preconditions(const std::set<Proposition_Node*>& found_precond,
      Action_Node* an);

    /// connect added nodes, create if necessary
    void connect_added_nodes(const Action& action, 
      std::set<Proposition_Node*>& new_props, Action_Node* an);

    /// connect deleted nodes, create if necessary
    void connect_deleted_nodes(const Action& action, 
      std::set<Proposition_Node*>& new_props, Action_Node* an);

    /// make mutex connections for actions
    void make_action_mutex_connections(std::set<Action_Node*>& new_actions,
      Action_Node* an);

    /// make mutex connections for propositions
    void make_proposition_mutex_connections(std::set<Proposition_Node*>& new_props);

    /// check for no mutex in set
    static bool is_mutex(const std::set<Proposition_Node*> props);

    /// starting propositions
    std::set<Proposition> starting_;

    /// goal propositions
    std::set<Proposition> goals_;

    /// available actions
    std::set<Action> actions_;

    /// prop nodes in graph
    std::set<Proposition_Node*> prop_nodes_;

    /// action nodes in graph
    std::set<Action_Node*> act_nodes_;
  }; // class Graphplan
} // namespace graphplan

#endif // _GRAPHPLAN_GRAPHPLAN_H_
