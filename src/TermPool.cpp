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

#include "TermPool.hpp"

#include "exceptions.hpp"

#include <iostream>
#include <stdlib.h>
#undef NDEBUG
#include <assert.h>

using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
TermPool::TermPool(IdentPool const& _idents)
 : idents(_idents),
   nino(1), neno(0x100), cap(inc), 
   data((ushort*)malloc(storage(cap)*sizeof(ushort))) {
  if(data == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  *data = 0;
}
TermPool::~TermPool() { free(data); }

//+ Data Buffer Increment ++++++++++++++++++++++++++++++++++++++++++++++++++++
inline void TermPool::grow() {
  data = (ushort*)realloc(data, storage(cap+=inc)*sizeof(ushort));
  if(data == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
}

//+ Terminal Definition ++++++++++++++++++++++++++++++++++++++++++++++++++++++
Symbol TermPool::
defineLiteral(unsigned char lit, ushort prec, Terminal::Assoc assoc) {
  assert(assoc < 3);

  if(nino > cap)  grow();
  ushort *ptr = data+index(nino);
  *ptr++ = nino;                  //- Internal Number
  *ptr++ = lit;                   //- External Number
  *ptr++ = 0;                     //- Literal -> no Ref to IdentPool
  *ptr++ = (prec << 2) | assoc;   //- Precedence / Associativity
  *ptr   = 0; //- End-Of-List
  return  Symbol(nino++);
}

Symbol TermPool::
defineToken(ushort ident, ushort prec, Terminal::Assoc assoc) {
  assert(assoc < 3);

  if(nino > cap)  grow();
  ushort *ptr = data+index(nino);
  *ptr++ = nino;                  //- Internal Number
  *ptr++ = neno++;                //- External Number
  *ptr++ = ident;                 //- Token -> Ref to IdentPool
  *ptr++ = (prec << 2) | assoc;   //- Precedence / Associativity
  *ptr   = 0; //- End-Of-List
  return  Symbol(nino++);
}

//+ Debugging / Visulaization ++++++++++++++++++++++++++++++++++++++++++++++++
void TermPool::dump(std::ostream& o) const {
  Iterator  it(iterator());

  o.setf(std::ios::right, std::ios::adjustfield);
  while(it) {
    Terminal  t(it.next());
    o.width(4);
    o << t.internal() << ": [";
    o.width(2);
    o << t.prec() << ", " << (t.isNone ()? "NONE" :
			      t.isRight()? "RGHT" : "LEFT") << "] "
      << t.toString(&idents) << std::endl;
  }
}
