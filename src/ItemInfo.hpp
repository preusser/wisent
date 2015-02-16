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

#ifndef WISENT_ITEMINFO_HPP_INCLUDED
#define WISENT_ITEMINFO_HPP_INCLUDED

#include "SymbolSet.hpp"
#include <set>

namespace wisent {
  class ItemInfo {
    typedef std::set<ItemInfo*>  Forwards;

    SymbolSet  lookaheads;
    Forwards   forwards;
    
    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    ItemInfo(unsigned lalen);
    ~ItemInfo();

  public:
    ItemInfo(ItemInfo const& o);

    //+ Lookahead Initialization +++++++++++++++++++++++++++++++++++++++++++++
  public:
    void addLookahead(Symbol la);
    void addForward(ItemInfo *target);

  public:
    SymbolSet const& getLookaheads() const { return  lookaheads; }

    //+ Lookahead Propagation ++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void propagate() const;
    void propagate(SymbolSet const& las);
  };
}
#endif
