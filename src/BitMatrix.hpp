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

#ifndef WISENT_BITMATRIX_HPP_INCLUDED
#define WISENT_BITMATRIX_HPP_INCLUDED

#include "BitSet.hpp"

namespace wisent {
  class BitMatrix {
    //+ Constants ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef BitSet::chunk  chunk;
    enum { CHUNK_LEN = 8*sizeof(chunk) };
    static unsigned clen(unsigned blen) { return (blen-1)/CHUNK_LEN + 1; }

    //+ Instance Members +++++++++++++++++++++++++++++++++++++++++++++++++++++
    unsigned const  rws;
    unsigned const  cls;
    chunk  **const  dat;

    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    BitMatrix(unsigned rows, unsigned cols);
    ~BitMatrix();

    //+ Dimensions +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    unsigned cols() const { return  cls; }
    unsigned rows() const { return  rws; }

    //+ Line Access ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    BitSet       operator[](unsigned row) const {
      return  BitSet(-cls, dat[row]);
    }
    BitSet const operator[](unsigned row) {
      return  BitSet(-cls, dat[row]);
    }
  };
}
#endif
