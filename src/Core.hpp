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

#ifndef WISENT_CORE_HPP_INCLUDED
#define WISENT_CORE_HPP_INCLUDED

#include "Item.hpp"

#include <set>
#include <iostream>

namespace wisent {
  class Core {
    typedef std::set<Item>  ItemSet;
    ItemSet  items;
    
  public:
    Core();
    ~Core();

  public:
    void add(Item const& i);
    bool has(Item const& i) const;
    
  public:
	typedef std::set<Item>::const_iterator  iterator;
    iterator begin() const { return  items.begin(); }
    iterator end  () const { return  items.end  (); }
    
  public:
    bool operator<(Core const& o) const;

  public:
    void dump(std::ostream& o, RulePool const *rules = 0) const;
  };
}
#endif
