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

#include "VarPool.hpp"

#include "IdentPool.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
VarPool::VarPool(IdentPool const& _idents)
 : idents(_idents),
   nvno(-1), cap(inc), 
   data((sshort*)malloc(storage(cap)*sizeof(sshort))) {
  if(data == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  *data = 0;
}
VarPool::~VarPool() { free(data); }

//+ Variable Definition ++++++++++++++++++++++++++++++++++++++++++++++++++++++
Symbol VarPool::defineVariable(sshort ident) {
  if(-nvno > (signed)cap) {
    data = (sshort*)realloc(data, storage(cap+=inc)*sizeof(ushort));
    if(data == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  }
  sshort *ptr = data+index(nvno);
  *ptr++ = nvno;                  //- Variable Number
  *ptr++ = ident;                 //- Ref to IdentPool
  *ptr   = 0; //- End-Of-List
  return  Symbol(nvno--);
}

void VarPool::dump(std::ostream& o) const {
  for(Iterator it(iterator()); it; it++) {
    o.setf(std::ios::right, std::ios::adjustfield);
    o.width(4);
    o << it->internal() << ": " << it->toString(&idents) << std::endl;
  }
}
