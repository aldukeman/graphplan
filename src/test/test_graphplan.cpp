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

void test_proposition()
{
  Proposition p_at_a_1("x_at_a");
  Proposition p_at_a_2("x_at_a");
  Proposition p_at_a_copy = p_at_a_1;
  assert(p_at_a_1 == p_at_a_copy);
  assert(p_at_a_1 == p_at_a_2);
  assert(p_at_a_1.get_name() == "x_at_a");
  stringstream expected;
  expected << "Proposition: x_at_a" << endl;
  assert(p_at_a_1.to_string() == expected.str());
}

void test_action()
{
  Proposition p_at_a("x_at_a");
  Proposition p_at_b("x_at_b");
  Action a_a_to_b("move_a_to_b");
  a_a_to_b.add_add(p_at_b);
  a_a_to_b.add_delete(p_at_a);
  a_a_to_b.add_precondition(p_at_a);

  assert(a_a_to_b.get_name() == "move_a_to_b");

  assert(*(a_a_to_b.get_adds().begin()) == p_at_b);
  auto it = a_a_to_b.get_adds().begin();
  ++it;
  assert(it == a_a_to_b.get_adds().end());

  assert(*(a_a_to_b.get_deletes().begin()) == p_at_a);
  it = a_a_to_b.get_deletes().begin();
  ++it;
  assert(it == a_a_to_b.get_deletes().end());

  assert(*(a_a_to_b.get_preconditions().begin()) == p_at_a);
  it = a_a_to_b.get_preconditions().begin();
  ++it;
  assert(it == a_a_to_b.get_preconditions().end());
}

void test_proposition_node()
{
  Proposition p_at_a("x_at_a");
  Proposition p_at_b("x_at_b");

  Proposition_Node pn_at_a(p_at_a);
  assert(pn_at_a.instance_of(p_at_a));
  assert(!pn_at_a.instance_of(p_at_b));
  assert(pn_at_a.get_name() == "x_at_a");
  assert(!pn_at_a.is_deleted());

  Proposition_Node pn_at_b(p_at_b, true);
  assert(pn_at_b.instance_of(p_at_b));
  assert(!pn_at_b.instance_of(p_at_a));
  assert(pn_at_b.get_name() == "x_at_b");
  assert(pn_at_b.is_deleted());
}

void test_action_node()
{
  Proposition p_at_a("x_at_a");
  Proposition p_at_b("x_at_b");
  Action a_a_to_b("move_a_to_b");
  a_a_to_b.add_add(p_at_b);
  a_a_to_b.add_delete(p_at_a);
  a_a_to_b.add_precondition(p_at_a);

  Action_Node an_a_to_b(a_a_to_b);

  assert(an_a_to_b.is_instance_of(a_a_to_b));
}

void test_graphplan()
{
  Proposition p_at_a("x_at_a");
  Proposition p_at_b("x_at_b");
  Action a_a_to_b("move_a_to_b");
  a_a_to_b.add_add(p_at_b);
  a_a_to_b.add_delete(p_at_a);
  a_a_to_b.add_precondition(p_at_a);

  Graphplan g;
  g.add_starting(p_at_a);
  g.add_goal(p_at_b);
  g.add_action(a_a_to_b);

  set<Proposition_Node*> props;

  // case 1: not goal
  assert(!g.goal_check(props));

  // case 2: has goal
  Proposition_Node pn_at_b(p_at_b);
  props.insert(&pn_at_b);
  assert(g.goal_check(props));

  // case 3: has more than goal
  Proposition_Node pn_at_a(p_at_a);
  props.insert(&pn_at_a);
  assert(g.goal_check(props));

  // case 4: mutex between them
  pn_at_a.add_mutex(&pn_at_b);
  pn_at_b.add_mutex(&pn_at_a);
  g.add_goal(p_at_a);
  assert(!g.goal_check(props));

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
  a_b_to_c.add_add(p_at_c);
  a_b_to_c.add_delete(p_at_b);
  a_b_to_c.add_precondition(p_at_b);
  test_2.add_action(a_a_to_b);
  test_2.add_action(a_b_to_c);
  assert(test_2.plan() == 2);

  // print out nodes
//  cout << "Proposition Nodes" << endl;
//  for(auto it = test.prop_nodes_.cbegin(); it != test.prop_nodes_.cend(); ++it)
//    cout << "\t" << (*it)->get_name() << endl;
//  cout << "Action Nodes" << endl;
//  for(auto it = test.act_nodes_.cbegin(); it != test.act_nodes_.cend(); ++it)
//    cout << "\t" << (*it)->get_name() << endl;
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
