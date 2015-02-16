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

#include "ItemInfo.hpp"

#undef NDEBUG
#include "assert.h"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
ItemInfo::ItemInfo(unsigned lalen) : lookaheads(lalen) {}
ItemInfo::~ItemInfo() {}

ItemInfo::ItemInfo(ItemInfo const& o) : lookaheads(o.lookaheads.length()) {
  assert(o.lookaheads.isEmpty());  // Allow copying only on empty infos
  assert(o.forwards.empty());
}

//+ Manipulation +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ItemInfo::addLookahead(Symbol la) {
  lookaheads[la] = true;
}

void ItemInfo::addForward(ItemInfo *target) {
  bool  neu = forwards.insert(target).second;
  assert(neu);
}

//+ Lookahead Propagation ++++++++++++++++++++++++++++++++++++++++++++++++++++
void ItemInfo::propagate() const {
  for(Forwards::const_iterator  it = forwards.begin();
      it != forwards.end();
      (*it++)->propagate(lookaheads));
}

void ItemInfo::propagate(SymbolSet const& las) {
  if(!las.subset(lookaheads)) {
    lookaheads.unify(las);
    propagate();
  }
}
