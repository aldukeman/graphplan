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
 * @file Graphplan_Parser.hpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 *
 * Graphplan parser class takes a string or file input and creates a Graphplan
 * object
 */

#ifndef _GRAPHPLAN_GRAPHPLAN_PARSER_H_
#define _GRAPHPLAN_GRAPHPLAN_PARSER_H_

#include <string>

#include "graphplan/Graphplan.hpp"

namespace graphplan
{
  class Graphplan_Parser
  {
  public:
    /// Constructor
    Graphplan_Parser();

    /// Destructor
    ~Graphplan_Parser();

    /// parse file
    bool parse_file(const std::string& file, Graphplan& g);

    /// parse string input
    bool parse_string(const std::string& input, Graphplan& g);

  protected:
    /// types of tokens expected in input
    enum Token_Type
    {
      INIT,
      COLON,
      EXCLAMATION,
      STRING,
      GOAL,
      ACTION,
      PRE,
      EFFECTS,
      END_STREAM,
      INVALID
    };

    /// storage for token types and actual text
    struct Token
    {
      Token();
      Token(const Token_Type& t, const std::string& te);
      Token_Type type;
      std::string text;
    };

    bool parse_graphplan(Graphplan& g);

    /// parse action pattern
    bool parse_action(Action& a);

    /// parse goal pattern
    bool parse_goal(std::set<Proposition>& goals);

    /// parse initialization pattern
    bool parse_init(std::set<Proposition>& init);

    /// parse proposition
    bool parse_proposition(Proposition& p);

    /// get next token
    Token get_next_token();

    /// parse proposition or reserved word
    void prop_reserve(Token& t);

    /// scanner members
    int line_;
    ifstream in_file_;
    istream* is_;
    char next_;
    bool backup_;
    int count_;

    /// parser members
    Token next_token_;

    /// reserved word 
    static const std::string INIT_STR;
    static const std::string GOAL_STR;
    static const std::string ACTION_STR;
    static const std::string PRE_STR;
    static const std::string EFFECTS_STR;
  }; // class Graphplan_Parser
} // namespace graphplan

#endif // _GRAPHPLAN_GRAPHPLAN_H_
