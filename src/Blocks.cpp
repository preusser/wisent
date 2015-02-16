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

#include "Blocks.hpp"

using namespace wisent;

Blocks::Blocks() {}
Blocks::~Blocks() {}

void Blocks::add(Kind kind, Block const& block) {
  blocks[kind] = block;
}
Block const& Blocks::get(Kind kind) const throw (NoEntryException) {
  Block const *res = find(kind);
  if(res == 0)  throw NoEntryException(__FILE__, __LINE__, "Blocks");
  return *res;
}

Block const *Blocks::find(Kind kind) const {
  BlockMap::const_iterator  it = blocks.find(kind);
  return (it == blocks.end())? 0 : &it->second;
}
