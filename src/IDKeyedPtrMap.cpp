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

#include "IDKeyedPtrMap.hpp"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
IDKeyedPMap::IDKeyedPMap() {}
IDKeyedPMap::~IDKeyedPMap() {}

//+ Insertion / Lookup +++++++++++++++++++++++++++++++++++++++++++++++++++++++
void *IDKeyedPMap::insert(char const *id, void *p) {
  PMap::iterator const  it = pmap.find(id);
  if(it == pmap.end()) {
    pmap[id] = p;
    return  0;
  }
  else {
    void *const  res = it->second;
    it->second = p;
    return  res;
  }
}

void *IDKeyedPMap::find(char const *id) const {
  PMap::const_iterator const  it = pmap.find(id);
  return (it == pmap.end())? 0 : it->second;
}

IDKeyedPMap::Iterator::Iterator(PMap const& _map)
  : pos(_map.begin()), end(_map.end()) {}
