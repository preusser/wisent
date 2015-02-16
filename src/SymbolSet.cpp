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

#include "SymbolSet.hpp"
#include "IdentPool.hpp"
#include "TermPool.hpp"
#include "VarPool.hpp"

#include <sstream>

using namespace wisent;

std::string SymbolSet::toString(IdentPool const *idents,
				TermPool  const *terms,
				VarPool   const *vars) const {
  std::string  res("{ ");

  unsigned const  len = length();
  if(len > 0) {
    if(operator[](0))  res += "EPS, ";
    unsigned  i = 1;

    if(terms) {
      unsigned const  trmCnt = terms->size();
      while((i <= trmCnt) && (i < len)) {
	if(operator[](i)) {
	  res += terms->get(i).toString(idents);
	  res += ", ";
	}
	i++;
      }
    }

    unsigned const  varCnt = vars? vars->size() : 0;
    while(i+varCnt < len) {
      if(operator[](i)) {
	std::ostringstream  o;
	o << i;
	res += o.str();
	res += ", ";
      }
      i++;
    }

    while(i < len) {
      if(operator[](i)) {
	res += vars->get(i-len).toString(idents);
	res += ", ";
      }
      i++;
    }

    res.resize(res.length()-2);
  }
  res += " }";
  return  res;
}
