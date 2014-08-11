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
 * @file Partial_Order_Plan.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * The Partial_Order_Plan class stores a partially ordered plan.
 */

#ifndef _GRAPHPLAN_PARTIAL_ORDER_PLAN_H_
#define _GRAPHPLAN_PARTIAL_ORDER_PLAN_H_

#include <vector>
#include <set>

#include "graphplan/Action.hpp"

namespace graphplan
{
  class Partial_Order_Plan
  {
  public:
    /// Constructor
    Partial_Order_Plan();

    /// add action at stage
    void add_action(const unsigned int& stage, const Action& a);

    /// get actions at all stages
    const std::vector<std::set<Action> >& get_actions() const;

    /// get actions at a specific stage
    const std::set<Action>& get_actions(unsigned int stage) const;

    /// get string representation
    std::string to_string() const;

  protected:
    /// actions in this plan
    std::vector<std::set<Action> > actions_;
  }; // class Partial_Order_Plan
} // namespace graphplan

#endif // _GRAPHPLAN_PARTIAL_ORDER_PLAN_H_
