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
 * @file Partial_Order_Plan.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 */

#include "graphplan/Partial_Order_Plan.hpp"

#include <sstream>
#include <iostream>

using std::stringstream;
using std::set;
using std::vector;
using std::cout;
using std::endl;

graphplan::Partial_Order_Plan::Partial_Order_Plan()
{
  actions_.resize(0);
}

void
graphplan::Partial_Order_Plan::add_action(const unsigned int& stage,
  const Action& a)
{
  if(actions_.size() <= stage)
    actions_.resize(stage + 1);
  actions_[stage].insert(a);
}

const std::vector<std::set<graphplan::Action> >&
graphplan::Partial_Order_Plan::get_actions() const
{
  return actions_;
}

const std::set<graphplan::Action>&
graphplan::Partial_Order_Plan::get_actions(unsigned int stage) const
{
  return actions_[stage];
}

std::string
graphplan::Partial_Order_Plan::to_string() const
{
  stringstream ret;
  unsigned int stage = 0;
  for(auto it = actions_.cbegin(); it != actions_.cend(); ++it)
  {
    ret << "Stage " << stage++ << endl;
    for(const Action a : *it)
    {
      ret << "\t" << a.get_name() << endl;
    }
  }

  return ret.str();
}
