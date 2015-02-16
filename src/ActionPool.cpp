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

#include "ActionPool.hpp"

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
ActionPool::ActionPool() {}
ActionPool::~ActionPool() {}

//+ Contents Access ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ActionPool::set(unsigned short rno, Block const& act) {
  unsigned const  s = acts.size();
  if(s == rno)  acts.push_back(act);
  else {
    if(s < rno)  acts.resize(rno+1);
    acts[rno] = act;
  }
}
