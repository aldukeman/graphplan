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
 * @file test_graphplan.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 */

#include <iostream>
#include <cassert>
#include <set>
#include <string>
#include <sstream>

#include "graphplan/Graphplan.hpp"
#include "graphplan/Action.hpp"
#include "graphplan/Proposition.hpp"

using std::cout;
using std::endl;
using std::set;
using std::string;
using std::stringstream;

using namespace graphplan;

Proposition p_at_a("x_at_a");
Proposition p_not_at_a("x_at_a", true);
Proposition p_at_b("x_at_b");
Proposition p_not_at_b("x_at_b", true);

void test_proposition()
{
  assert(p_at_a == p_at_a);
  assert(p_at_a != p_not_at_a);
  assert(p_at_a.is_negation_of(p_not_at_a));
}

void test_action()
{
  Action a_a_to_b("move_a_to_b");
  a_a_to_b.add_precondition(p_at_a);
  a_a_to_b.add_effect(p_at_b);
  a_a_to_b.add_effect(p_not_at_a);

  assert(a_a_to_b.get_name() == "move_a_to_b");
}

void test_proposition_node()
{
  Proposition_Node pn_at_a(p_at_a);
  assert(pn_at_a.instance_of(p_at_a));
  assert(!pn_at_a.instance_of(p_at_b));
  assert(pn_at_a.get_name() == "x_at_a");

  Proposition_Node pn_at_b(p_at_b);
  assert(pn_at_b.instance_of(p_at_b));
  assert(!pn_at_b.instance_of(p_at_a));
  assert(pn_at_b.get_name() == "x_at_b");
}

void test_action_node()
{
  Action a_a_to_b("move_a_to_b");
  a_a_to_b.add_precondition(p_at_a);
  a_a_to_b.add_effect(p_at_b);
  a_a_to_b.add_effect(p_not_at_a);

  Action_Node an_a_to_b(a_a_to_b);
  assert(an_a_to_b.is_instance_of(a_a_to_b));
}

void test_graphplan()
{
  Action a_a_to_b("move_a_to_b");
  a_a_to_b.add_precondition(p_at_a);
  a_a_to_b.add_effect(p_at_b);
  a_a_to_b.add_effect(p_not_at_a);

  Graphplan g;
  g.add_starting(p_at_a);
  g.add_goal(p_at_b);
  g.add_action(a_a_to_b);

  set<Proposition_Node*> props;

  // case 1: not goal
  set<Action_Node*> actions;
  assert(!g.goal_check(props, actions));

  // case 2: has goal
  Proposition_Node pn_at_b(p_at_b);
  props.insert(&pn_at_b);
  assert(g.goal_check(props, actions));

  // case 3: has more than goal
  Proposition_Node pn_at_a(p_at_a);
  props.insert(&pn_at_a);
  assert(g.goal_check(props, actions));

  // case 4: mutex between them
  pn_at_a.add_mutex(&pn_at_b);
  pn_at_b.add_mutex(&pn_at_a);
  g.add_goal(p_at_a);
  assert(!g.goal_check(props, actions));

  // attempt a plan
  Graphplan test;
  test.add_starting(p_at_a);
  test.add_goal(p_at_b);
  test.add_action(a_a_to_b);
  assert(test.plan() == 1);

  // attempt another plan
  Graphplan test_2;
  test_2.add_starting(p_at_a);
  Proposition p_at_c("p_at_c");
  test_2.add_goal(p_at_c);
  Action a_b_to_c("move_b_to_c");
  a_b_to_c.add_precondition(p_at_b);
  a_b_to_c.add_effect(p_at_c);
  a_b_to_c.add_effect(p_not_at_b);
  test_2.add_action(a_a_to_b);
  test_2.add_action(a_b_to_c);
  assert(test_2.plan() == 2);

  // birthday dinner example
  Graphplan birthday;
  Proposition garb("garb");
  Proposition not_garb("garb", true);
  Proposition dinner("dinner");
  Proposition not_dinner("dinner", true);
  Proposition present("present");
  Proposition not_present("present", true);
  Proposition clean("clean");
  Proposition not_clean("clean", true);
  Proposition quiet("quiet");
  Proposition not_quiet("quiet", true);
  birthday.add_starting(garb);
  birthday.add_starting(clean);
  birthday.add_starting(quiet);
  birthday.add_goal(not_garb);
  birthday.add_goal(dinner);
  birthday.add_goal(present);
  Action cook("cook");
  cook.add_precondition(clean);
  cook.add_effect(dinner);
  birthday.add_action(cook);
  Action wrap("wrap");
  wrap.add_precondition(quiet);
  wrap.add_effect(present);
  birthday.add_action(wrap);
  Action carry("carry");
  carry.add_precondition(garb);
  carry.add_effect(not_garb);
  carry.add_effect(not_clean);
  birthday.add_action(carry);
  Action dolly("dolly");
  dolly.add_precondition(garb);
  dolly.add_effect(not_garb);
  dolly.add_effect(not_quiet);
  birthday.add_action(dolly);
  cout << birthday.plan(10) << endl;
}

int main()
{
  test_proposition();
  test_action();
  test_proposition_node();
  test_action_node();
  test_graphplan();

  return 0;
}
