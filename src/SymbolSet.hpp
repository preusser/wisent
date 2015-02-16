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

#ifndef WISENT_SYMBOLSET_HPP_INCLUDED
#define WISENT_SYMBOLSET_HPP_INCLUDED

#include "Symbol.hpp"
#include "BitSet.hpp"

#include <string>

namespace wisent {
  class IdentPool;
  class TermPool;
  class VarPool;
  class SymbolMatrix;

  class SymbolSet : public BitSet {
  public:
    SymbolSet(unsigned len) : BitSet(len) {}
    ~SymbolSet() {}

  private:
    SymbolSet(BitSet const& set) : BitSet(set) {}
    friend class SymbolMatrix;

  public:
    BRef       operator[](Symbol sym) {
      return  sym.isVariable()? BitSet::operator[](length()+sym) :
	                        BitSet::operator[](sym);
    }
    BRef const operator[](Symbol sym) const {
      return  sym.isVariable()? BitSet::operator[](length()+sym) :
	                        BitSet::operator[](sym);
    }

  public:
    std::string toString(IdentPool const *idents = 0,
			 TermPool  const *terms  = 0,
			 VarPool   const *vars   = 0) const;
  };
}
#endif
