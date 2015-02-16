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

#ifndef WISENT_LR1ITEM_HPP_INCLUDED
#define WISENT_LR1ITEM_HPP_INCLUDED

#include "Item.hpp"
#include "Symbol.hpp"

namespace wisent {
  class LR1Item : public Item {
    /** lookahead */
    Symbol  la;

  public:
    LR1Item(Item _lr0, Symbol _la) : Item(_lr0), la(_la) {}
    ~LR1Item() {}

  public:
    bool operator<(LR1Item const& o) const {
      return    Item::operator<(o)?       true :
                o.Item::operator<(*this)? false : (la < o.la);
    }

  public:
    Symbol lookahead() const { return  la; }
  };
}
#endif
