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
 * @file Proposition.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * A Proposition in Graphplan
 */

#include "graphplan/Proposition.hpp"

#include <string>
#include <sstream>

using std::stringstream;
using std::endl;
using std::string;

graphplan::Proposition::Proposition(const std::string& n) :
  name_(n)
{
}

bool
graphplan::Proposition::operator==(const Proposition& p) const
{
  return name_.compare(p.name_) == 0;
}

bool
graphplan::Proposition::operator!=(const Proposition& p) const
{
  return name_.compare(p.name_) != 0;
}

bool
graphplan::Proposition::operator<(const Proposition& p) const
{
  return name_ < p.name_;
}

const string&
graphplan::Proposition::get_name() const
{
  return name_;
}

string
graphplan::Proposition::to_string() const
{
  stringstream ret;
  ret << "Proposition: " << name_ << endl;
  return ret.str();
}