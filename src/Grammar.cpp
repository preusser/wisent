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

#include "Grammar.hpp"

#include "Frontend.hpp"
#include "SymbolSet.hpp"
#include "SymbolMatrix.hpp"
#include "Collection.hpp"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
Grammar::Grammar(Frontend& src) : terms(idents), vars(idents), rules(terms, vars) {
  // Build grammar from source
  Builder  bld(*this);
  src.build(bld);
}

Grammar::~Grammar() {}

//+ Inspection +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Grammar::echo(std::ostream& o) const {
  o << "Idents:\n\n";    idents.dump(o);
  o << "Terminals:\n\n"; terms .dump(o);
  o << "Variables:\n\n"; vars  .dump(o);
  o << "Rules:\n\n";     rules .dump(o);
}

void Grammar::warn(std::ostream& o) const {
  // Check variable/terminal usage
  SymbolSet  lhs(1 + vars.size());                 // vars + EPS/$
  SymbolSet  rhs(1 + vars.size() + terms.size());  // vars + EPS/$ + terms

  for(RulePool::Iterator  it = rules.iterator(0); it.hasNext();) {
    Rule const  rule = it.next();
    
    lhs[rule.lhs()] = true;
    for(unsigned i = rule.length(); i-- > 0;) {
      Symbol const  sym = rule.rhs(i);
      rhs[sym] = true;
    }
  }
  
  for(signed var = -vars.size(); var < 0; var++) {
    if(!lhs[var]) {
      o << "Warning: undefined variable \"" 
	<< vars.get(var).toString(&idents) << '\"' << std::endl;
    }
    if(!rhs[var]) {
      o << "Warning: unused variable \""
	<< vars.get(var).toString(&idents) << '\"' << std::endl;
    }
  }
  
  for(unsigned term = terms.size(); term > 0; term--) {
    if(!rhs[term]) {
      o << "Warning: unused terminal \""
	<< terms.get(term).toString(&idents) << '\"' << std::endl;
    }
  }
}

void Grammar::analyze(std::ostream& o) {
  unsigned const  trmCnt = terms.size();
  unsigned const  varCnt = vars .size();

  SymbolMatrix  reach(varCnt, varCnt+1+trmCnt);
  SymbolMatrix  first(varCnt, varCnt+1+trmCnt);

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Reachability (not skipping epsilon)

  // Adjacency: A -> X ...
  for(RulePool::Iterator  it = rules.iterator(); it.hasNext();) {
    Rule const  rule = it.next();
    reach[rule.lhs()][rule.rhs(0)] = true;
  }

  // Reachability: A => X ... (without deleting epsilons)
  for(signed  dst = -varCnt; dst < 0; dst++) {
    SymbolSet  dstLine = reach[dst];
    for(signed  src = -varCnt; src < 0; src++) {
      SymbolSet  srcLine = reach[src];
      if(srcLine[dst])  srcLine.unify(dstLine);
    }
    dstLine[dst] = true;
  }

  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // FIRST: Reachability skipping epsilons

  // A derives epsilon: A => EPS
  {
    bool  changed;
    do {
      changed = false;

      // Scan all rules
      for(RulePool::Iterator  it = rules.iterator(); it.hasNext();) {
	Rule const  rule = it.next();
	SymbolSet   set  = first[rule.lhs()];

	if(!set[0]) { // Left-Hand Side not yet known to derive epsilon
	  Symbol    rhs;
	  unsigned  i = 0;
	  do { rhs = rule.rhs(i++); } while(rhs.isVariable() && first[rhs][0]);
	  if(rhs.isEpsilon()) {
	    set[0]  = true;
	    changed = true;
	  }
	}
      }
    }
    while(changed);
  }

  // "Adjacency": A -> X .. (deleting possible epsilons in front of X)
  for(RulePool::Iterator  jt = rules.iterator(); jt.hasNext();) {
    Rule const  rule = jt.next();
    SymbolSet   set  = first[rule.lhs()];

    Symbol    rhs;
    unsigned  i = 0;
    do { 
      set[rhs = rule.rhs(i++)] = true;
    }
    while(rhs.isVariable() && first[rhs][0]);
  }

  // FIRST: A => X ... (with deleting epsilons)
  for(signed  dest = -varCnt; dest < 0; dest++) {
    for(signed  src = -varCnt; src < 0; src++) {
      SymbolSet  srcLine = first[src];
      if(srcLine[dest])  srcLine.unifyNZ(first[dest]);
    }
  }

  o << "REACH:\n\n"; reach.dump(o, &terms, &vars);
  o << "FIRST:\n\n"; first.dump(o, &terms, &vars);
  
  Collection  coll(terms, vars, rules, reach, first);
  coll.dump(o);
  coll.fill(table);
}
