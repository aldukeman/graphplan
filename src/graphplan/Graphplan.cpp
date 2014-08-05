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
 * @file Graphplan.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * Graphplan
 */

#include "graphplan/Graphplan.hpp"

#include <sstream>
#include <iostream>

#include "graphplan/Proposition.hpp"
#include "graphplan/Action.hpp"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::endl;
using std::set;
using std::vector;

graphplan::Goal::Goal(const Proposition& p, const bool& d) :
  prop(p), del(d)
{
}

bool graphplan::Goal::operator<(const Goal& g) const
{
  return prop < g.prop;
}

graphplan::Graphplan::Graphplan()
{
}

graphplan::Graphplan::~Graphplan()
{
  for(auto it = prop_nodes_.cbegin(); it != prop_nodes_.cend(); ++it)
    delete *it;
  for(auto it = act_nodes_.cbegin(); it != act_nodes_.cend(); ++it)
    delete *it;
}

void
graphplan::Graphplan::add_starting(const Proposition& p)
{
  starting_.insert(p);
}

void
graphplan::Graphplan::add_goal(const Proposition& p, const bool& deleted)
{
  goals_.insert(Goal(p, deleted));
}

void
graphplan::Graphplan::add_action(const Action& a)
{
  actions_.insert(a);
}

const set<graphplan::Proposition>&
graphplan::Graphplan::get_starting() const
{
  return starting_;
}

const set<graphplan::Action>&
graphplan::Graphplan::get_actions() const
{
  return actions_;
}

const set<graphplan::Goal>&
graphplan::Graphplan::get_goals() const
{
  return goals_;
}

unsigned int
graphplan::Graphplan::plan(unsigned int iterations)
{
  // init proposition nodes
  set<Proposition_Node*> props;
  for(auto it = starting_.cbegin(); it != starting_.cend(); ++it)
  {
    Proposition_Node* p = new Proposition_Node(it->get_name());
    prop_nodes_.insert(p);
    props.insert(p);
  }

  // while not at goal, perform another iteration
  unsigned int iter;
  for(iter = 0; iter < iterations && !goal_check(props); ++iter)
  {
    set<Proposition_Node*> new_props;
    set<Action_Node*> new_acts;
    iteration(props, new_props, new_acts);

    for(set<Proposition_Node*>::const_iterator it = new_props.cbegin();
      it != new_props.cend(); ++it)
    {
      prop_nodes_.insert(*it);
    }
    for(set<Action_Node*>::const_iterator it = new_acts.cbegin();
      it != new_acts.cend(); ++it)
    {
      act_nodes_.insert(*it);
    }

    props.swap(new_props);
  }

  return iter;
}

string
graphplan::Graphplan::to_string() const
{
  stringstream ret;
  ret << "Starting Propositions:" << endl;
  for(auto it = starting_.cbegin(); it != starting_.cend(); ++it)
    ret << "\t" << it->get_name() << endl;

  ret << "Goal Propositions:" << endl;
  for(auto it = goals_.cbegin(); it != goals_.cend(); ++it)
    ret << "\t" << it->prop.get_name() << (it->del ? " deleted" : "") << endl;

  ret << "Actions:" << endl;
  for(auto it = actions_.cbegin(); it != actions_.cend(); ++it)
  {
    ret << "\t" << it->get_name() << endl;
    ret << "\t\tPreconditions:" << endl;
    auto vec1 = it->get_preconditions();
    for(auto it2 = vec1.cbegin(); it2 != vec1.cend(); ++it2)
      ret << "\t\t\t" << it2->get_name() << endl;
    ret << "\t\tAdds:" << endl;
    auto vec2 = it->get_adds();
    for(auto it2 = vec2.cbegin(); it2 != vec2.cend(); ++it2)
      ret << "\t\t\t" << it2->get_name() << endl;
    ret << "\t\tDeletes:" << endl;
    auto vec3 = it->get_deletes();
    for(auto it2 = vec3.cbegin(); it2 != vec3.cend(); ++it2)
      ret << "\t\t\t" << it2->get_name() << endl;
  }

  return ret.str();
}

void
graphplan::Graphplan::iteration(const set<Proposition_Node*>& props,
  set<Proposition_Node*>& new_props, set<Action_Node*>& new_actions)
{
  // copy each of the nodes
  for(set<Proposition_Node*>::const_iterator it = props.cbegin();
    it != props.cend(); ++it)
  {
    new_props.insert(new Proposition_Node(**it));
  }

  // foreach action
  for(set<Action>::const_iterator action = actions_.cbegin();
    action != actions_.cend(); ++action)
  {
    // foreach precondition
    const set<Proposition>& preconds = action->get_preconditions();
    bool good = true;
    set <Proposition_Node*> found_precond;
    for(set<Proposition>::const_iterator precond = preconds.cbegin();
      precond != preconds.cend() && good; ++precond)
    {
      // foreach proposition, check for match
      good = false;
      for(set<Proposition_Node*>::const_iterator prop = props.cbegin();
        prop != props.cend() && !good; ++prop)
      {
        good = (*prop)->instance_of(*precond) && (!(*prop)->is_deleted());
        if(good)
          found_precond.insert(*prop);
      }
    }

    // if all preconditions are present and not mutex
    if(good && !(is_mutex(found_precond)))
    {
      // create action node and add result nodes
      Action_Node* an = new Action_Node(*action);
      connect_preconditions(found_precond, an);
      connect_added_nodes(*action, new_props, an);
      connect_deleted_nodes(*action, new_props, an);
      make_action_mutex_connections(new_actions, an);

      new_actions.insert(an);
    }
  }

  make_proposition_mutex_connections(new_props);
}

bool
graphplan::Graphplan::goal_check(const set<Proposition_Node*>& props) const
{
  // foreach goal proposition
  set<Proposition_Node*> found_goals;
  for(set<Goal>::const_iterator goal = goals_.cbegin(); goal != goals_.cend();
    ++goal)
  {
    // search for goal in propositions
    set<Proposition_Node*>::const_iterator it;
    for(it = props.cbegin(); it != props.cend(); ++it)
    {
      if((*it)->instance_of(goal->prop) && ((*it)->is_deleted() == goal->del))
        break;
    }

    // if we haven't found goal, then we are not in a goal state
    if(it == props.cend())
      return false;

    // add goal to found
    found_goals.insert(*it);
  }

  // ensure no mutex
  return !(is_mutex(found_goals));
}

bool
graphplan::Graphplan::is_mutex(const std::set<Proposition_Node*> props)
{
  // ensure no mutex
  for(auto g_1 = props.cbegin(); g_1 != props.cend(); ++g_1)
  {
    auto g_2 = g_1;
    for(++g_2; g_2 != props.cend(); ++g_2)
    {
      if((*g_1)->get_mutex().find(*g_2) != (*g_1)->get_mutex().cend())
        return true;
    }
  }

  // if we reach here, then nothing is mutex
  return false;
}

void
graphplan::Graphplan::connect_preconditions(
  const set<Proposition_Node*>& found_precond, Action_Node* an)
{
  for(set<Proposition_Node*>::const_iterator precond = 
    found_precond.cbegin(); precond != found_precond.cend(); ++precond)
  {
    an->add_precondition(*precond);
    (*precond)->add_supply(an);
  }
}

void
graphplan::Graphplan::connect_added_nodes(const Action& action, 
  set<Proposition_Node*>& new_props, Action_Node* an)
{
  const set<Proposition>& added = action.get_adds();
  for(set<Proposition>::const_iterator add = added.cbegin();
    add != added.cend(); ++add)
  {
    // check if proposition is already added
    set<Proposition_Node*>::const_iterator pro;
    for(pro = new_props.cbegin(); pro != new_props.cend(); ++pro)
    {
      if((*pro)->instance_of(*add) && !(*pro)->is_deleted())
        break;
    }

    // if already in ret set, then just connect to node
    if(pro != new_props.cend())
    {
      (*pro)->add_cause(an);
      an->add_add(*pro);
    }
    else
    {
      Proposition_Node* p = new Proposition_Node(*add, false);
      p->add_cause(an);
      new_props.insert(p);
      an->add_add(p);
    }
  }
}

void
graphplan::Graphplan::connect_deleted_nodes(const Action& action, 
  set<Proposition_Node*>& new_props, Action_Node* an)
{
  const set<Proposition>& added = action.get_adds();
  for(set<Proposition>::const_iterator add = added.cbegin();
    add != added.cend(); ++add)
  {
    // check if proposition is already added
    set<Proposition_Node*>::const_iterator pro;
    for(pro = new_props.cbegin(); pro != new_props.cend(); ++pro)
    {
      if((*pro)->instance_of(*add) && (*pro)->is_deleted())
        break;
    }

    // if already in ret set, then just connect to node
    if(pro != new_props.cend())
    {
      an->add_add(*pro);
      (*pro)->add_cause(an);
    }
    else
    {
      Proposition_Node* p = new Proposition_Node(*add, true);
      p->add_cause(an);
      new_props.insert(p);
      an->add_add(p);
    }
  }
}

void
graphplan::Graphplan::make_action_mutex_connections(
  set<Action_Node*>& new_actions, Action_Node* an)
{
  // foreach other action
  for(set<Action_Node*>::iterator other_action = new_actions.begin();
    other_action != new_actions.end(); ++other_action)
  {
    /**
     * case 1: inconsistent effects - an action adds a proposition effect
     *         that another action deletes
     */
    const set<Proposition_Node*>& my_adds = an->get_adds();
    const set<Proposition_Node*>& other_deletes =
      (*other_action)->get_deletes();
    for(set<Proposition_Node*>::const_iterator add = my_adds.cbegin();
      add != my_adds.cend(); ++add)
    {
      for(set<Proposition_Node*>::const_iterator del = other_deletes.cbegin();
        del != other_deletes.cend(); ++del)
      {
        if((*add)->instance_of((*del)->get_proposition()))
        {
          (*other_action)->add_mutex(an);
          an->add_mutex(*other_action);
        }
      }
    }

    /**
     * case 2: interference - an action deletes a proposition effect that
     *         another action adds
     */
    const set<Proposition_Node*>& my_dels = an->get_deletes();
    const set<Proposition_Node*>& other_adds =
      (*other_action)->get_adds();
    for(set<Proposition_Node*>::const_iterator add = my_dels.cbegin();
      add != my_dels.cend(); ++add)
    {
      for(set<Proposition_Node*>::const_iterator del = other_adds.cbegin();
        del != other_adds.cend(); ++del)
      {
        if((*add)->instance_of((*del)->get_proposition()))
        {
          (*other_action)->add_mutex(an);
          an->add_mutex(*other_action);
        }
      }
    }

    /**
     * case 3: competing needs - an action has preconditions that are mutex with
     *         another actions preconditions
     */
    const set<Proposition_Node*>& my_pre = an->get_preconditions();
    const set<Proposition_Node*>& other_pre =
      (*other_action)->get_preconditions();
    for(set<Proposition_Node*>::const_iterator mine = my_pre.cbegin();
      mine != my_pre.cend(); ++mine)
    {
      for(set<Proposition_Node*>::const_iterator other = other_pre.cbegin();
        other != other_pre.cend(); ++other)
      {
        // if props are same...
        if((*mine)->instance_of((*other)->get_proposition()))
        {
          // if props are not both deleted or both present...
          if((*mine)->is_deleted() != (*other)->is_deleted())
          {
            // if they are mutex...
            const set<Proposition_Node*>& mutex = (*mine)->get_mutex();
            if(mutex.find(*other) != mutex.cend())
            {
              // ...then actions are mutex
              (*other_action)->add_mutex(an);
              an->add_mutex(*other_action);
            }
          }
        }
      }
    }
  }
}

void
graphplan::Graphplan::make_proposition_mutex_connections(
  set<Proposition_Node*>& new_props)
{
  /**
   * case 1: propositions are mutex if they are negations of each other
   */
  for(set<Proposition_Node*>::iterator prop_1 = new_props.begin();
    prop_1 != new_props.end(); ++prop_1)
  {
    auto prop_2 = prop_1;
    for(++prop_2; prop_2 != new_props.end(); ++prop_2)
    {
      // if same proposition...
      if((*prop_1)->instance_of((*prop_2)->get_proposition()))
      {
        // if props are different is_deleted...
        if((*prop_1)->is_deleted() != (*prop_2)->is_deleted())
        {
          // ...then props are mutex
          (*prop_1)->add_mutex(*prop_2);
          (*prop_2)->add_mutex(*prop_1);
        }
      }
    }
  }

  /**
   * case 2: all actions that could have created the proposition are pairwise
   *         mutex
   */
  for(set<Proposition_Node*>::iterator prop_1 = new_props.begin();
    prop_1 != new_props.end(); ++prop_1)
  {
    set<Proposition_Node*>::iterator prop_2 = prop_1;
    for(++prop_2; prop_2 != new_props.end(); ++prop_2)
    {
      // get mutual causes
      const set<Action_Node*>& causes_1 = (*prop_1)->get_cause();
      const set<Action_Node*>& causes_2 = (*prop_2)->get_cause();
      set<Action_Node*> mutual_causes;
      for(set<Action_Node*>::const_iterator act_1 = causes_1.cbegin();
        act_1 != causes_1.cend(); ++act_1)
      {
        for(set<Action_Node*>::const_iterator act_2 = causes_2.cbegin();
          act_2 != causes_2.cend(); ++act_2)
        {
          if(*act_1 == *act_2)
          {
            mutual_causes.insert(*act_1);
            break;
          }
        }
      }

      // check pairwise actions for mutex
      if(mutual_causes.size() >= 2)
      {
        bool found_pair = false;
        for(set<Action_Node*>::const_iterator act_1 = mutual_causes.cbegin();
          act_1 != mutual_causes.cend() && !found_pair; ++act_1)
        {
          set<Action_Node*>::const_iterator act_2 = act_1;
          for(++act_2; act_2 != mutual_causes.cend() && !found_pair; ++act_2)
          {
            set<Action_Node*> mutex_set = (*act_1)->get_mutex();
            found_pair = (mutex_set.find(*act_2) == mutex_set.cend());
          }
        }
    
        if(!found_pair)
        {
          (*prop_1)->add_mutex(*prop_2);
          (*prop_2)->add_mutex(*prop_1);
        }
      }
    }
  }
}
