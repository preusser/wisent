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

#include "SymbolMatrix.hpp"

#include "IdentPool.hpp"
#include "TermPool.hpp"
#include "VarPool.hpp"

using namespace wisent;

//- Visualization ------------------------------------------------------------
void SymbolMatrix::dump(std::ostream& o, 
			TermPool  const *terms, VarPool const *vars) const {
  unsigned const  rws = rows();
  unsigned const  cls = cols();
  unsigned  i = 0;

  o << "    ";
  if(i++ < cls) {
    o << "EPS ";
    if(terms) {
      unsigned const  trmCnt = terms->size();
      while((i < cls) && (i <= trmCnt))  {
	o << terms->toString(i++, 4);
      }
    }
    unsigned const  varCnt = vars? vars->size() : 0;
    while(i+varCnt < cls) {
      o.width(4);
      o << i++;
    }

    while(i < cls) {
      o << vars->toString(i++-cls, 4);
    }
  }
  o << std::endl;  // End: Header

  for(unsigned k = 0; k < rws;) {
    BitSet  s = BitMatrix::operator[](k);
    if(vars)  o << vars->toString(-++k, 4);
    else {
      o.width(4);
      o << k;
    }

    for(unsigned j = 0; j < cls; o << (s[j++]? " X  " : "    "));
    o << std::endl;
  }
}
