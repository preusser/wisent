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

#include "Symbol.hpp"

#include "TermPool.hpp"
#include "VarPool.hpp"

#include <sstream>

using namespace wisent;

std::string Symbol::toString(TermPool  const *terms,
			     VarPool   const *vars,
			     unsigned         width) const {
  if(isEpsilon ()) {
    if(width == 0)  width = 1;
    std::string  res(width, ' ');
    res[width-1] = '$';
    return  res;
  }
  if(isTerminal() && (terms != 0))  return  terms->toString(no, width);
  if(isVariable() && (vars  != 0))  return  vars ->toString(no, width);
  
  std::ostringstream  res;
  if(width != 0)  res.width(width);
  res << no;
  return  res.str();
}
