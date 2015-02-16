/***
 * wisent - LALR(1) Parser Generator
 * Copyright (C) 2004,2005 Thomas B. Preuﬂer <preusser@ite.inf.tu-dresden.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef WISENT_RULEPOOL_HPP_INCLUDED
#define WISENT_RULEPOOL_HPP_INCLUDED

#include "types.hpp"
#include "Rule.hpp"
#include <iostream>

namespace wisent {
  class Context;
  class IdentPool;
  class TermPool;
  class VarPool;

  class RulePool {
    //+ Data Members +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  private:
    TermPool const& terms;
    VarPool  const& vars;

    /** increment (in ushorts) for growth of insufficient buffer */
    enum { dinc = 256 };
    /** current capacity (in ushorts) of data buffer */
    unsigned  dcap;
    /** storage for all rules */
    sshort *data;

    enum { iinc = 32 };
    unsigned  icap;
    sshort **index;
    
    /** next available rule number */
    ushort  nrno;

    /** end of list  */
    sshort *eol;

    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    RulePool(TermPool const& _terms, VarPool const& _vars);
    ~RulePool();
  private: // not to be implemented
    RulePool(RulePool const& o);
    RulePool& operator=(RulePool const& o);

    //+ Size +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    unsigned short size() const { return  nrno; }

    //+ Growth of dynamic memory +++++++++++++++++++++++++++++++++++++++++++++
  private:
    void grow();

    //+ Definition of Rules ++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    /** Determines the start symbol of the grammar. */
    void   startSymbol(Symbol var);

    /** Starts the construction of a new rule for variable var. */
    void   startRule (Symbol var);
    /** Appends sym to the end of the current rule under construction. */
    void   appendRule(Symbol sym);
    /** Closes the rule under construction and returns its rule number. */
    ushort finishRule(ushort prec);

    //+ Access to Rules ++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //- Single Rule ----------------------------------------------------------
  public:
    Rule get(ushort rno) const { return  Rule(index[rno]); }
    std::string toString(ushort rno) const {
      return  get(rno).toString(&terms, &vars);
    }
    std::string toString(Rule rule) const {
      return  rule.toString(&terms, &vars);
    }
    std::string toString(ushort rno, unsigned dot) const {
      return  get(rno).toString(&terms, &vars, dot);
    }
    std::string toString(Rule rule, unsigned dot) const {
      return  rule.toString(&terms, &vars, dot);
    }
    
    //- Iteration ------------------------------------------------------------
    class Iterator {
      sshort const *ptr;

    public:
      Iterator(sshort const *p) : ptr(p) {}
      ~Iterator() {}

    public:
      operator bool() const { return  hasNext(); }
      Rule const  operator* () const { return  Rule(ptr); }
      Rule const *operator->() const {
	return  reinterpret_cast<Rule const*>(this);
      }
      Iterator& operator++()    { ptr += ptr[2] + 5;  return *this; }
      Iterator  operator++(int) { 
	Iterator  res(ptr);  ptr += ptr[2] + 5;  return  res;
      }

    public:
      bool hasNext() const { return *ptr != -1; }
      Rule next() { Rule  res(ptr);  ptr += ptr[2] + 5;  return  res; }
    };
    Iterator iterator(unsigned from = 1) const { 
      return  Iterator(index[from]);
    }

    //+ Debugging ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void dump(std::ostream& o) const;
  };
}
#endif
