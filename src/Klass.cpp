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

#include "Klass.hpp"

using namespace wisent;

Klass::Klass() {}
Klass::Klass(std::string const& klass, std::string const& _adjunct) 
  : adjunct(_adjunct) {
  size_t  apos = 0;

  while(true) {
    size_t const  epos = klass.find_first_of(".:", apos);
    if(epos == std::string::npos)  break;
    spaces.push_back(klass.substr(apos, epos-apos));
    apos = epos + ((klass[epos+1] == ':')? 2 : 1);
  }
  name = klass.substr(apos);
}
Klass::~Klass() {}

std::string Klass::getScope(std::string const& delim) const {
  std::string  res;
  if(spaces.size() > 0) {
    std::vector<std::string>::const_iterator  it = spaces.begin();
    while(true) {
      res += *it++;
      if(it == spaces.end())  break;
      res += delim;
    }
  }
  return  res;
}

std::string Klass::toString(std::string const& delim) const {
  return  (cntSpaces() > 0)? (getScope(delim) + delim + name) : name;
}
