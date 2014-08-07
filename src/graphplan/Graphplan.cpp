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
#include <queue>

#include "graphplan/Proposition.hpp"
#include "graphplan/Action.hpp"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::endl;
using std::set;
using std::queue;
using std::map;

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
graphplan::Graphplan::add_starting(const Proposition& p, const bool& deleted)
{
  starting_.insert(Starting(p, deleted));
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

const set<graphplan::Starting>&
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
  cout << "Initial nodes:" << endl;
  set<Proposition_Node*> props;
  for(set<Starting>::iterator it = starting_.cbegin(); it != starting_.cend();
    ++it)
  {
    Proposition_Node* p = new Proposition_Node(it->prop);
    prop_nodes_.insert(p);
    props.insert(p);
    cout << "\t" << p->to_string() << endl;
  }

  // while not at goal, perform another iteration
  unsigned int iter;
  set<Action_Node*> actions;
  for(iter = 0; iter < iterations && !goal_check(props, actions); ++iter)
  {
    cout << "iteration: " << iter << endl;

    set<Proposition_Node*> new_props;
    set<Action_Node*> new_acts;
    iteration(props, new_props, new_acts);

    // store proposition nodes for later deletion
    for(set<Proposition_Node*>::const_iterator it = new_props.cbegin();
      it != new_props.cend(); ++it)
    {
      prop_nodes_.insert(*it);
    }

    // store action nodes for later deletion
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
    ret << "\t" << it->prop.get_name() << (it->del ? " deleted" : "") << endl;

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
      ret << "\t\t\t" << it2->to_string() << endl;
    ret << "\t\tEffects:" << endl;
    auto vec2 = it->get_effects();
    for(auto it2 = vec2.cbegin(); it2 != vec2.cend(); ++it2)
      ret << "\t\t\t" << it2->to_string() << endl;
  }

  return ret.str();
}

void
graphplan::Graphplan::iteration(const set<Proposition_Node*>& props,
  set<Proposition_Node*>& new_props, set<Action_Node*>& new_actions)
{
  // handle maintenance actions
  // copy each of the nodes
  for(set<Proposition_Node*>::iterator it = props.cbegin();
    it != props.cend(); ++it)
  {
    Proposition p = (*it)->get_proposition();
    Proposition_Node* pn = new Proposition_Node(p);
    Action maintenance;
    maintenance.add_precondition(p);
    maintenance.add_effect(p);
    Action_Node* maint = new Action_Node("maintenance_" + (*it)->get_name());
    maint->add_precondition(*it);
    maint->add_effect(pn);
    pn->add_cause(maint);
    make_action_mutex_connections(new_actions, maint);
    new_props.insert(pn);
    new_actions.insert(maint);
  }

  // foreach action
  for(set<Action>::const_iterator action = actions_.cbegin();
    action != actions_.cend(); ++action)
  {
    cout << "checking action " << action->get_name() << endl;

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
        good = (*prop)->instance_of(*precond);
        if(good)
          found_precond.insert(*prop);
      }
    }

    // if all preconditions are present and not mutex
    if(good && !(is_mutex(found_precond)))
    {
      cout << "\tadding action" << endl;
      // create action node and add result nodes
      Action_Node* an = new Action_Node(*action);
      connect_preconditions(found_precond, an);
      connect_effect_nodes(*action, new_props, an);
      make_action_mutex_connections(new_actions, an);

      new_actions.insert(an);
    }
  }

  make_proposition_mutex_connections(new_props);

  cout << "propositions:" << endl;
  for(auto it = new_props.cbegin(); it != new_props.cend(); ++it)
  {
    cout << "\t" << (*it)->to_string() << endl;
    auto mutex = (*it)->get_mutex();
    cout << "\t\tmutex:" << endl;
    for(auto mut = mutex.cbegin(); mut != mutex.cend(); ++mut)
    {
      cout << "\t\t\t" << (*mut)->to_string() << endl;
    }
  }
}

bool
graphplan::Graphplan::goal_check(const set<Proposition_Node*>& props,
  set<Action_Node*>& /*actions*/) const
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
      if((*it)->instance_of(goal->prop))
        break;
    }

    // if we haven't found goal, then we are not in a goal state
    if(it == props.cend())
      return false;

    // add goal to found
    found_goals.insert(*it);
  }

  // ensure no mutex
  if(!(is_mutex(found_goals)))
  {
    map<const Proposition_Node*, Action_Node*> prop_causes;
    return level_goal_check(found_goals, prop_causes);
  }

  return false;
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
graphplan::Graphplan::connect_effect_nodes(const Action& action, 
  set<Proposition_Node*>& new_props, Action_Node* an)
{
  const set<Proposition>& effects = action.get_effects();
  for(set<Proposition>::const_iterator effect = effects.cbegin();
    effect != effects.cend(); ++effect)
  {
    // check if proposition is already added
    set<Proposition_Node*>::const_iterator pro;
    for(pro = new_props.cbegin(); pro != new_props.cend(); ++pro)
    {
      if((*pro)->instance_of(*effect))
        break;
    }

    // if already in ret set, then just connect to node
    if(pro != new_props.cend())
    {
      (*pro)->add_cause(an);
      an->add_effect(*pro);
    }
    else
    {
      Proposition_Node* p = new Proposition_Node(*effect);
      new_props.insert(p);
      p->add_cause(an);
      an->add_effect(p);
    }
  }
}

void
graphplan::Graphplan::make_action_mutex_connections(
  set<Action_Node*>& new_actions, Action_Node* an)
{
  // foreach other actions
  for(Action_Node* other_action : new_actions)
  {
    const set<Proposition_Node*>& my_effects = an->get_effects();
    const set<Proposition_Node*>& my_preconditions = an->get_preconditions();
    const set<Proposition_Node*>& other_effects = other_action->get_effects();
    const set<Proposition_Node*>& other_preconditions = 
      other_action->get_preconditions();
    /**
     * case 1: inconsistent effects - an action adds a proposition effect
     *         that another action deletes or is negation of
     */
    cout << "check for inconsistent effects with " << other_action->get_name() << endl;
    for(const Proposition_Node* mine : my_effects)
    {
      for(const Proposition_Node* other : other_effects)
      {
        if(mine->get_proposition().is_negation_of(other->get_proposition()))
        {
          other_action->add_mutex(an);
          an->add_mutex(other_action);
          cout << "\tfound" << endl;
        }
      }
    }

    /**
     * case 2: interference - an action deletes a preconditon that another 
     *         action needs
     */
    cout << "check for interference with " << other_action->get_name() << endl;
    for(const Proposition_Node* mine : my_preconditions)
    {
      for(const Proposition_Node* other : other_effects)
      {
        if(mine->get_proposition().is_negation_of(other->get_proposition()))
        {
          other_action->add_mutex(an);
          an->add_mutex(other_action);
          cout << "\tfound" << endl;
        }
      }
    }
    for(const Proposition_Node* mine : my_effects)
    {
      for(const Proposition_Node* other : other_preconditions)
      {
        if(mine->get_proposition().is_negation_of(other->get_proposition()))
        {
          other_action->add_mutex(an);
          an->add_mutex(other_action);
          cout << "\tfound" << endl;
        }
      }
    }

    /**
     * case 3: competing needs - an action has preconditions that are mutex with
     *         another actions preconditions
     */
    cout << "check for competing needs with " << other_action->get_name() << endl;
    for(const Proposition_Node* mine : my_preconditions)
    {
      for(const Proposition_Node* other : other_preconditions)
      {
        const set<const Proposition_Node*>& mutex = mine->get_mutex();
        if(mutex.find(other) != mutex.cend())
        {
          other_action->add_mutex(an);
          an->add_mutex(other_action);
          cout << "\tfound" << endl;
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
    set<Proposition_Node*>::iterator prop_2 = prop_1;
    for(++prop_2; prop_2 != new_props.end(); ++prop_2)
    {
      if((*prop_1)->get_proposition().is_negation_of(
        (*prop_2)->get_proposition()))
      {
        (*prop_1)->add_mutex(*prop_2);
        (*prop_2)->add_mutex(*prop_1);
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
      // get causes
      const set<Action_Node*>& causes_1 = (*prop_1)->get_causes();
      const set<Action_Node*>& causes_2 = (*prop_2)->get_causes();
      set<Action_Node*> causes;
      for(Action_Node* act_1 : causes_1)
        causes.insert(act_1);
      for(Action_Node* act_2 : causes_2)
        causes.insert(act_2);

      // check pairwise actions for mutex
      if(causes.size() >= 2)
      {
        bool found_pair = false;
        for(set<Action_Node*>::const_iterator act_1 = causes.cbegin();
          act_1 != causes.cend() && !found_pair; ++act_1)
        {
          set<Action_Node*>::const_iterator act_2 = act_1;
          for(++act_2; act_2 != causes.cend() && !found_pair; ++act_2)
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

bool
graphplan::Graphplan::level_goal_check(const set<Proposition_Node*>& props,
  map<const Proposition_Node*, Action_Node*> /*prop_causes*/)
{
  // check if we are at level 0
  set<Proposition_Node*>::const_iterator p = props.cbegin();
  if((*p)->get_causes().size() == 0)
    return true;

  // recursively call sub_level_goal_check
  map<const Proposition_Node*, Action_Node*> causes;
  return sub_level_goal_check(props, props.cbegin(), causes);
}

bool
graphplan::Graphplan::sub_level_goal_check(const set<Proposition_Node*>& props,
  set<Proposition_Node*>::const_iterator cur,
  map<const Proposition_Node*, Action_Node*> prop_causes)
{
  // check if done recursing in this function
  if(cur == props.cend())
  {
    set<Proposition_Node*> new_props;
    for(const Proposition_Node* p : props)
      for(Proposition_Node* cause : prop_causes[p]->get_preconditions())
        new_props.insert(cause);
    map<const Proposition_Node*, Action_Node*> new_prop_causes;
    return level_goal_check(new_props, new_prop_causes);
  }

  // find action for next proposition
  for(Action_Node* act : (*cur)->get_causes())
  {
    // check if it's mutex with other already selected actions
    set<Action_Node*> mutex_actions = act->get_mutex();
    bool mutex = false;
    for(Proposition_Node* p : props)
    {
      if(mutex_actions.find(prop_causes[p]) != mutex_actions.cend())
      {
        mutex = true;
        break;
      }
    }

    if(!mutex)
    {
      prop_causes[*cur] = act;
      auto next = cur;
      ++next;
      if(sub_level_goal_check(props, next, prop_causes))
      {
        return true;
      }
      prop_causes[*cur] = 0;
    }
  }

  return false;
}
