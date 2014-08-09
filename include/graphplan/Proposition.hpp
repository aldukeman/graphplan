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
 * @file Proposition.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * A Proposition in Graphplan
 */

#ifndef _GRAPHPLAN_PROPOSITION_H_
#define _GRAPHPLAN_PROPOSITION_H_

#include <string>

namespace graphplan
{
  class Proposition
  {
  public:
    /// Constructor
    Proposition(const std::string& n = "", const bool& neg = false);

    /// equality operator
    bool operator==(const Proposition& p) const;

    /// inequality operator
    bool operator!=(const Proposition& p) const;

    /// ordering operator
    bool operator<(const Proposition& p) const;

    /// get proposition name
    const std::string& get_name() const;

    /// get negated
    bool is_negated() const;

    /// determine if this is a negation of another proposition
    bool is_negation_of(const Proposition& p) const;

    /// set negated status
    void set_negated(const bool& n = true);

    /// set name
    void set_name(const std::string& n);

    /// get string version
    std::string to_string() const;

  protected:
    /// name of the proposition
    std::string name_;

    /// is negated
    bool negated_;
  }; // class Proposition
} // namespace graphplan

#endif // _GRAPHPLAN_PROPOSITION_H_
