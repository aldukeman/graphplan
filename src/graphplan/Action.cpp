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
 * @file Action.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * An Action in Graphplan
 */

#include "graphplan/Action.hpp"

#include <sstream>

using std::stringstream;
using std::endl;
using std::string;
using std::set;

graphplan::Action::Action(const string& n) : name_(n)
{
}

bool
graphplan::Action::operator==(const Action& a) const
{
  return name_ == a.name_;
}

bool
graphplan::Action::operator<(const Action& a) const
{
  return name_ < a.name_;
}

void
graphplan::Action::add_effect(const Proposition& p)
{
  effects_.insert(p);
}

void
graphplan::Action::add_precondition(const Proposition& p)
{
  preconditions_.insert(p);
}

const string&
graphplan::Action::get_name() const
{
  return name_;
}

const set<graphplan::Proposition>&
graphplan::Action::get_effects() const
{
  return effects_;
}

const set<graphplan::Proposition>&
graphplan::Action::get_preconditions() const
{
  return preconditions_;
}

string
graphplan::Action::to_string() const
{
  stringstream ret;
  ret << "Action: " << name_ << endl;

  ret << "\tPreconditions:" << endl;
  for(const Proposition& pre : preconditions_)
    ret << "\t\t" << pre.to_string() << endl;

  ret << "\tEffects:" << endl;
  for(const Proposition& effect : effects_)
    ret << "\t\t" << effect.to_string() << endl;

  return ret.str();
}

void
graphplan::Action::set_name(const std::string& n)
{
  name_ = n;
}
