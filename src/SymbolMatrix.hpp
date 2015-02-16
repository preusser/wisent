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

#ifndef WISENT_SYMBOLMATRIX_HPP_INCLUDED
#define WISENT_SYMBOLMATRIX_HPP_INCLUDED

#include "SymbolSet.hpp"
#include "BitMatrix.hpp"

#include <iostream>

//#define NDEBUG
#include "assert.h"

namespace wisent {
  class IdentPool;
  class TermPool;
  class VarPool;

  class SymbolMatrix : public BitMatrix {
  public:
    SymbolMatrix(unsigned rows, unsigned cols) : BitMatrix(rows, cols) {}
    ~SymbolMatrix() {}

  public:
    SymbolSet       operator[](Symbol sym)       {
      assert(sym.isVariable());
      return  SymbolSet(BitMatrix::operator[](~sym));
    }
    SymbolSet const operator[](Symbol sym) const {
      assert(sym.isVariable());
      return  SymbolSet(BitMatrix::operator[](~sym));
    }

  public:
    void dump(std::ostream& o,
	      TermPool  const *terms  = 0,
	      VarPool   const *vars   = 0) const;
  };
}
#endif
