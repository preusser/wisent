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

#include "Variable.hpp"

#include "IdentPool.hpp"
#include <sstream>

using namespace wisent;

std::string Variable::toString(IdentPool const *idents, unsigned width) const {
  if(idents) {
    std::string  res(idents->get(ident()));
    if(width)  res.resize(width, ' ');
    return  res;
  }
  else {
    std::ostringstream  res;
    res.setf(std::ios::right, std::ios::adjustfield);
    if(width)  res.width(width);
    res << internal();
    return  res.str();
  }
}
