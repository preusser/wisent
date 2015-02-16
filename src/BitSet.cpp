/***
 * wisent - LALR(1) Parser Generator
 * Copyright (C) 2004,2005 Thomas B. Preußer <preusser@ite.inf.tu-dresden.de>
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

#include "BitSet.hpp"
#include "exceptions.hpp"

//#define NDEBUG
#include "assert.h"

using wisent::BitSet;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
BitSet::BitSet(unsigned _len) : len(_len), dat(new chunk[clen(len)]) {
  if(dat == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  for(chunk *p = dat+clen(len); p > dat; *--p = 0);
}

BitSet::~BitSet() {
  if(len > 0)  delete [] dat;
}

//+ Clearing +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void BitSet::clear() {
  for(chunk *p = dat+clen(len); p > dat; *--p = 0);
}

bool BitSet::isEmpty() const {
  for(chunk *p = dat+clen(len); p > dat;)  if(*--p != 0)  return  false;
  return  true;
}

//+ Set Operations +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BitSet& BitSet::unify(BitSet const& o) {
  unsigned const l = (len > 0)? len : -len;
  assert(l == unsigned((o.len > 0)? o.len : -o.len));

  for(unsigned  i = clen(l); i-- > 0; dat[i] |= o.dat[i]);
  return *this;
}

BitSet& BitSet::unifyNZ(BitSet const& o) {
  unsigned const l = (len > 0)? len : -len;
  assert(l == unsigned((o.len > 0)? o.len : -o.len));

  for(unsigned  i = clen(l); --i > 0; dat[i] |= o.dat[i]);
  if(l > 0)  dat[0] |= o.dat[0] & (~0u << 1);
  return *this;
}

bool BitSet::subset(BitSet const& o) const {
  unsigned const l = (len > 0)? len : -len;
  assert(l == unsigned((o.len > 0)? o.len : -o.len));

  for(unsigned  i = clen(l); i-- > 0;) {
    if((dat[i] & ~o.dat[i]) != 0)  return  false;
  }
  return  true;
}
