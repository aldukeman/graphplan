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
 * @file Graphplan_Parser.cpp
 * @author Anton Dukeman <anton.dukeman@gmail.com>
 */

#include "graphplan/Graphplan_Parser.hpp"

#include <sstream>

using std::string;
using std::istringstream;
using std::set;
using std::cout;
using std::endl;

const string graphplan::Graphplan_Parser::ACTION_STR = "ACTION";
const string graphplan::Graphplan_Parser::INIT_STR = "INIT";
const string graphplan::Graphplan_Parser::GOAL_STR = "GOAL";
const string graphplan::Graphplan_Parser::PRE_STR = "PRE";
const string graphplan::Graphplan_Parser::EFFECTS_STR = "EFFECTS";

graphplan::Graphplan_Parser::Graphplan_Parser() :
  line_(0), backup_(false), count_(0)
{
}

graphplan::Graphplan_Parser::~Graphplan_Parser()
{
}

bool
graphplan::Graphplan_Parser::parse_file(const string& file, Graphplan& g)
{
  in_file_.open(file);
  is_ = &in_file_;
  return parse_graphplan(g);
}

bool
graphplan::Graphplan_Parser::parse_string(const string& input, Graphplan& g)
{
  istringstream input_stream(input);
  is_ = &input_stream;
  return parse_graphplan(g);
}

graphplan::Graphplan_Parser::Token::Token() :
  type(INVALID), text("")
{
}

graphplan::Graphplan_Parser::Token::Token(const Token_Type& t,
  const string& te) :
  type(t), text(te)
{
}

bool
graphplan::Graphplan_Parser::error(const Token_Type& t) const
{
  if(t == INIT)
    return error("INIT");
  if(t == COLON)
    return error("COLON");
  if(t == EXCLAMATION)
    return error("EXCLAMATION");
  if(t == STRING)
    return error("STRING");
  if(t == GOAL)
    return error("GOAL");
  if(t == ACTION)
    return error("ACTION");
  if(t == PRE)
    return error("PRE");
  if(t == EFFECTS)
    return error("EFFECTS");
  if(t == END_STREAM)
    return error("END_STREAM");
  return error("INAVLID");
}

bool
graphplan::Graphplan_Parser::error(const string& t) const
{
  cout << line_ << ":" << count_ << " found token type " << next_token_.type <<
    " but expected " << t << endl;
  return false;
}

bool
graphplan::Graphplan_Parser::parse_graphplan(Graphplan& g)
{
  next_token_ = get_next_token();
  while(next_token_.type != END_STREAM)
  {
    switch(next_token_.type)
    {
      case INIT:
      {
        set<Proposition> init;
        if(parse_init(init))
        {
          for(Proposition p : init)
            g.add_starting(p);
        }
        else
          return false;
        break;
      }
      case GOAL:
      {
        set<Proposition> goal;
        if(parse_goal(goal))
        {
          for(Proposition p : goal)
            g.add_goal(p);
        }
        else
          return false;
        break;
      }
      case ACTION:
      {
        Action a;
        if(parse_action(a))
          g.add_action(a);
        else
          return false;
        break;
      }
      default:
        return error("INIT, GOAL, or ACTION");
    }
  }

  return true;
}

bool
graphplan::Graphplan_Parser::parse_init(set<Proposition>& init)
{
  if(next_token_.type != INIT)
    return error(INIT);

  next_token_ = get_next_token();
  if(next_token_.type != COLON)
    return error(COLON);

  next_token_ = get_next_token();
  while(next_token_.type == EXCLAMATION || next_token_.type == STRING)
  {
    Proposition p;
    if(parse_proposition(p))
      init.insert(p);
    else
      return false;
    next_token_ = get_next_token();
  }

  return true;
}

bool
graphplan::Graphplan_Parser::parse_goal(set<Proposition>& goals)
{
  if(next_token_.type != GOAL)
    return error(GOAL);

  next_token_ = get_next_token();
  if(next_token_.type != COLON)
    return error(COLON);

  next_token_ = get_next_token();
  while(next_token_.type == EXCLAMATION || next_token_.type == STRING)
  {
    Proposition p;
    if(parse_proposition(p))
      goals.insert(p);
    else
      return false;
    next_token_ = get_next_token();
  }
  return true;
}

bool
graphplan::Graphplan_Parser::parse_action(Action& a)
{
  if(next_token_.type != ACTION)
    return error(ACTION);

  next_token_ = get_next_token();
  if(next_token_.type != COLON)
    return error(COLON);

  next_token_ = get_next_token();
  if(next_token_.type != STRING)
    return error(STRING);
  a.set_name(next_token_.text);

  next_token_ = get_next_token();
  if(next_token_.type != PRE)
    return error(PRE);

  next_token_ = get_next_token();
  if(next_token_.type != COLON)
    return error(COLON);

  next_token_ = get_next_token();
  while(next_token_.type == EXCLAMATION || next_token_.type == STRING)
  {
    Proposition p;
    if(parse_proposition(p))
      a.add_precondition(p);
    else
      return false;
    next_token_ = get_next_token();
  }

  if(next_token_.type != EFFECTS)
    return error(EFFECTS);

  next_token_ = get_next_token();
  if(next_token_.type != COLON)
    return error(COLON);

  next_token_ = get_next_token();
  while(next_token_.type == EXCLAMATION || next_token_.type == STRING)
  {
    Proposition p;
    if(parse_proposition(p))
      a.add_effect(p);
    else
      return false;
    next_token_ = get_next_token();
  }
  
  return true;
}

bool
graphplan::Graphplan_Parser::parse_proposition(Proposition& p)
{
  if(next_token_.type == EXCLAMATION)
  {
    p.set_negated(true);
    next_token_ = get_next_token();
  }
  else
  {
    p.set_negated(false);
  }

  if(next_token_.type == STRING)
    p.set_name(next_token_.text);
  else
    return error(STRING);

  return true;
}

graphplan::Graphplan_Parser::Token
graphplan::Graphplan_Parser::get_next_token()
{
  Token t(INVALID, "");

  if(backup_)
  {
    backup_ = false;
  }
  else
  {
    next_ = is_->get();
    ++count_;
  }

  while(true)
  {
    switch(next_)
    {
      case EOF:
      {
        t.type = END_STREAM;
        return t;
      }
      case '\n':
        ++line_;
        count_ = 0;
      case '\r':
      case ' ':
      case '\t':
      {
        next_ = is_->get();
        break;
      }
      case ':':
      {
        t.type = COLON;
        return t;
      }
      case '!':
      {
        t.type = EXCLAMATION;
        return t;
      }
      default:
      {
        prop_reserve(t);
        return t;
      }
    }
  }
}

void
graphplan::Graphplan_Parser::prop_reserve(Token& t)
{
  string text = "";
  if(isalpha(next_))
  {
    do
    {
      text = text + next_;
      next_ = is_->get();
      ++count_;
    }
    while(isalpha(next_) || isdigit(next_) || next_ == '_');
    backup_ = true;

    if(text == INIT_STR)
    {
      t.type = INIT;
    }
    else if(text == GOAL_STR)
    {
      t.type = GOAL;
    }
    else if(text == ACTION_STR)
    {
      t.type = ACTION;
    }
    else if(text == PRE_STR)
    {
      t.type = PRE;
    }
    else if(text == EFFECTS_STR)
    {
      t.type = EFFECTS;
    }
    else // STRING
    {
      t.type = STRING;
      t.text = text;
    }
  }
  else
  {
    t.type = INVALID;
  }
}
