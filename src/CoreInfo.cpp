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

#include "CoreInfo.hpp"
#include "ItemInfo.hpp"

#undef NDEBUG
#include "assert.h"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
CoreInfo::CoreInfo(unsigned _index, unsigned _lalen)
  : index(_index), lalen(_lalen) {}
CoreInfo::~CoreInfo() {}

//+ Manipulation +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void CoreInfo::addGoto(Symbol sym, CoreTuple *core) {
  bool const  neu = gotos.insert(GotoMap::value_type(sym, core)).second;
  assert(neu);
}

CoreInfo::Goto CoreInfo::getGoto(Symbol sym) {
  GotoMap::iterator  it = gotos.find(sym);
  assert(it != gotos.end());
  return  it;
}

wisent::ItemInfo& CoreInfo::getInfo(Item const& i) {
  InfoMap::iterator  it = infos.find(i);
  if(it == infos.end()) {
    it = infos.insert(InfoMap::value_type(i, ItemInfo(lalen))).first;
  }
  return  it->second;
}

wisent::ItemInfo const& CoreInfo::getInfo(Item const& i) const {
  InfoMap::const_iterator  it = infos.find(i);
  return  it->second;
}

void CoreInfo::addReduction(unsigned short rno, Symbol sym) {
  reduces.insert(Reduction(rno, sym));
}

//+ Lookahead Propagation ++++++++++++++++++++++++++++++++++++++++++++++++++++
void CoreInfo::propagate() {
  for(InfoMap::iterator  it = infos.begin(); 
      it != infos.end();
      it++->second.propagate());
}
