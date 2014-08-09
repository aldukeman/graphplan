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
 * @file Action.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * An Action in Graphplan
 */

#ifndef _GRAPHPLAN_ACTION_H_
#define _GRAPHPLAN_ACTION_H_

#include <string>
#include <set>

#include "graphplan/Proposition.hpp"

namespace graphplan
{
  class Action
  {
  public:
    /// Constructor
    Action(const std::string& n = "");

    /// equality operator
    bool operator==(const Action& a) const;

    /// less than operator
    bool operator<(const Action& a) const;

    /// add delete
    void add_effect(const Proposition& p);

    /// add precondition
    void add_precondition(const Proposition& p);

    /// get proposition name
    const std::string& get_name() const;

    /// get adds
    const std::set<Proposition>& get_effects() const;

    /// get preconditions
    const std::set<Proposition>& get_preconditions() const;

    /// get string version
    std::string to_string() const;

    /// set action name
    void set_name(const std::string& n);

  protected:
    /// name of the proposition
    std::string name_;

    /// added propositions
    std::set<Proposition> effects_;

    /// required propositions
    std::set<Proposition> preconditions_;
  }; // class Action
} // namespace graphplan

#endif // _GRAPHPLAN_ACTION_H_
