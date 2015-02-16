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

#include "ActionTable.hpp"

#include "TermPool.hpp"
#include "RulePool.hpp"

#undef NDEBUG
#include <assert.h>

using namespace wisent;

//+ Construction +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ActionTable::reset(unsigned coreCnt, unsigned trmCnt, unsigned varCnt) {
  height = coreCnt;
  width  = varCnt + 1 + trmCnt;
  offset = varCnt;

  unsigned const  size = height*width;
  data   = new sshort[size];
  for(unsigned i = size; i > 0; data[--i] = 0);
}

//+ High-Level Builder +++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ActionTable::Builder::selectRow(ushort state) {
  core = state;
  row  = table[state];
}

void ActionTable::Builder::shift(Symbol sym, ushort state) {
  signed short const  old = row[sym];

  assert(sym.isTerminal());
  assert(state > 1);
  assert(old <= 0); // shift/shift conflicts are impossible by construction

  if(old < 0) {     // shift/reduce conflict
    bool const  keep = shift_reduce(sym, -old) != SHIFT;
    conflict(core, sym, keep? old:state, keep? state:old);
    if(keep)  return;
  }
  row[sym] = state;
}

void ActionTable::Builder::reduce(Symbol sym, ushort rule) {
  assert(!sym.isVariable());

  signed short const  old = row[sym];
  signed short const  neu = -rule;

  if(old != 0) {
    bool const  keep =
      (old > 0)? shift_reduce(sym, rule) != REDUCE :  // shift/reduce
                 neu <= old;  // reduce/reduce - lowest rule number wins
    conflict(core, sym, keep? old:neu, keep? neu:old);
    if(keep)  return;
  }
  row[sym] = (neu == 0)? 1 : neu;
}

void ActionTable::Builder::go2(Symbol sym, ushort state) {
  assert(sym.isVariable());
  assert(row[sym] == 0);
  row[sym] = state;
}

ActionTable::Builder::SHIFT_REDUCE
ActionTable::Builder::shift_reduce(Symbol sym, ushort rno) const {
  unsigned short const  rprec = rules.get(rno).prec();
  Terminal       const  term  = terms.get(sym);
  unsigned short const  tprec = term.prec();
  
  // Shift unless precedences are specified and 
  //   - rule precedence is greater than terminal precedence or
  //   - precedences equal and terminal is left associative
  return  ((rprec != 0) && (tprec != 0) &&
           ((rprec > tprec) || ((rprec == tprec) && term.isLeft())))?
    REDUCE : SHIFT;
}

void ActionTable::Builder::
conflict(ushort state, Symbol sym, sshort win, sshort loose) {
  std::clog << state << "; " << sym.toString(&terms) << ": ";

  if(win < 0) std::clog << 'R' << -win;
  else  std::clog << 'S' << win;
  std::clog << " over ";
  if(loose < 0)  std::clog << 'R' << -loose;
  else  std::clog << 'S' << loose;
  std::clog << '\n';

  if(win   < 0)  std::clog << rules.toString(-  win) << '\n';
  if(loose < 0)  std::clog << rules.toString(-loose) << '\n';
  std::clog << std::endl;
}
