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

#include "Rule.hpp"

#include "SymbolMatrix.hpp"

#include "IdentPool.hpp"
#include "TermPool.hpp"
#include "VarPool.hpp"
#include <sstream>

using namespace wisent;

void Rule::first(SymbolMatrix const& fm, SymbolSet& f, ushort idx) const {
  sshort const *p = ptr+4+idx;

  f.clear();
  while(*p < 0) {
    SymbolSet  src(fm[Symbol(*p++)]);
    f.unifyNZ(src);
    if(!src[0])  return;
  }
  f[Symbol(*p)] = true;
}

//+ Debugging / Visualization ++++++++++++++++++++++++++++++++++++++++++++++++
std::string Rule::toString(TermPool const *terms,
			   VarPool  const *vars,
			   unsigned const  dot) const {
  std::ostringstream  res;

  // Rule number
  res.setf(std::ios::right, std::ios::adjustfield);
  res.width(4);
  res << number() << ": [";

  // Precedence
  res.width(2);
  res << prec() << "] ";

  // Left-hand Side
  if(lhs()) {
    if(vars)  res << vars->toString(lhs(), 8);
    else {
      res.setf(std::ios::right, std::ios::adjustfield);
      res.width(8);
      res << lhs();
    }
  }
  else  res << "$       ";

  // Right-hand Side
  res << " ->";
  if(dot == 0)  res << " .";

  unsigned  i = 0;
  while(true) {
    Symbol const  sym = rhs(i++);
    if(sym.isEpsilon())  break;
    res << ' ' << sym.toString(terms, vars);
    if(dot == i)  res << " .";
  }
  return  res.str();
}
