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

#include "Core.hpp"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
Core::Core() {}
Core::~Core() {}

//+ Element Access +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Core::add(Item const& i)       { items.insert(i); }
bool Core::has(Item const& i) const { return  items.find(i) != items.end(); }

//+ Comparison +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool Core::operator<(Core const& o) const {
  {  // by size
    unsigned const  as =   items.size();
    unsigned const  bs = o.items.size();
    if(as < bs)  return  true;
    if(bs < as)  return  false;
  }{ // by elements
    iterator  ai =   items.begin();
    iterator  bi = o.items.begin();
    while(ai != items.end()) {
      if(*ai < *bi)  return  true;
      if(*bi < *ai)  return  false;
      ai++, bi++;
    }
  }  // equal
  return  false;
}

//+ Visualization ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Core::dump(std::ostream& o, RulePool  const *rules) const {
  for(iterator  it = items.begin(); it != items.end();) {
    o << it++->toString(rules) << std::endl;
  }
}
