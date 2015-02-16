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

#include "IdentPool.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <stdlib.h>
#include <string.h>

using wisent::IdentPool;

IdentPool::IdentPool() : dcap(dinc), data ((char*) malloc(dcap*sizeof(char))),
			 icap(iinc), index((char**)malloc(icap*sizeof(char*))),
			 nino(1) {
  if((data == 0) || (index == 0)) {
    free(data);  data  = 0;
    free(index); index = 0;
    throw  OutOfMemoryException(__FILE__, __LINE__);
  }

  index[0] = data;  // always points to End-Of-List
  index[1] = 0;     // End-Of-Index
}
IdentPool::~IdentPool() { 
  free(data);
  free(index);
}


unsigned IdentPool::add(char const *ident) {
  unsigned  l = strlen(ident) + 1;

  // Truncate identifier to length of dinc-1 (terminating '\0')
  if(l > dinc)  l = dinc;

  // Grow data area if necessary
  if(data + dcap < *index + l) {
    char const *const  old = data;
    data = (char*)realloc(data,  (dcap+=dinc)*sizeof(char));
    if(data == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
    if(data != old) {
      unsigned const  d = data-old;
      for(unsigned i = 0; i < nino; index[i++] += d);
    }
  }

  // Grow index if necessary
  if(nino+1 >= icap) {
    index = (char**)realloc(index, (icap+=iinc)*sizeof(char*));
    if(index == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  }
  memcpy(index[nino] = *index, ident, l);
  *index += l;
  index[nino+1] = 0;

  return  nino++;
}

void IdentPool::dump(std::ostream& o) const {
  o.setf(std::ios::right, std::ios::adjustfield);
  for(unsigned i = 1; i < nino; i++) {
    o.width(4);
    o << i << ": \"" << index[i] << '\"' << std::endl;
  }
}

/*
int main() {
  IdentPool  idents;

  idents.add("abc");
  idents.add("asdjflkajdfl");
  idents.add("sdfalsjflkjakldjfklajsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("sdfakdjfhaljdndskjnafknfkljsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("abc");
  idents.add("asdjflkajdfl");
  idents.add("sdfalsjflkjakldjfklajsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("sdfakdjfhaljdndskjnafknfkljsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("abc");
  idents.add("asdjflkajdfl");
  idents.add("sdfalsjflkjakldjfklajsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("sdfakdjfhaljdndskjnafknfkljsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("abc");
  idents.add("asdjflkajdfl");
  idents.add("sdfalsjflkjakldjfklajsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("sdfakdjfhaljdndskjnafknfkljsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("abc");
  idents.add("asdjflkajdfl");
  idents.add("sdfalsjflkjakldjfklajsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("sdfakdjfhaljdndskjnafknfkljsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("abc");
  idents.add("asdjflkajdfl");
  idents.add("sdfalsjflkjakldjfklajsklfjklasjdflkjadncv,mnxasdjflkajdfl");
  idents.add("sdfakdjfhaljdndskjnafknfkljsklfjklasjdflkjadncv,mnxasdjflkajdfl");

  idents.dump(std::cout);
}
*/
