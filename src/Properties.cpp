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

#include "Properties.hpp"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
char const *Properties::getProperty(char const *name) const
  throw (NoEntryException) {
  char const *res = findProperty(name);
  if(res == 0)  throw  NoEntryException(__FILE__, __LINE__, name);
  return  res;
}

char const *Properties::findProperty(char const *name) const {
  char const *res = entries.find(name);
  return (res != 0)? res : (fallback == 0)? 0 : fallback->findProperty(name);
}

//+ Dump +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Properties::dump(std::ostream& o) const {
  Props::Iterator  it = entries.iterator();
  while(it.hasNext()) {
    std::pair<char const *, char const *> const  entry = it.next();
    o << entry.first << ":\t" << entry.second << '\n';
  }
}
