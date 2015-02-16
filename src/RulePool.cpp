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

#include "RulePool.hpp"

#include "TermPool.hpp"
#include "VarPool.hpp"

#include "SymbolMatrix.hpp"

#include "exceptions.hpp"

#include <sstream>
#include <stdlib.h>

#undef NDEBUG
#include <assert.h>

using namespace wisent;

RulePool::RulePool(TermPool const& _terms, VarPool const& _vars)
 : terms(_terms), vars(_vars),
   dcap(dinc), data ((sshort*) malloc(dcap*sizeof(sshort))),
   icap(iinc), index((sshort**)malloc(icap*sizeof(sshort*))),
		       nrno(1) {
  if((data == 0) || (index == 0)) {
    free(data);  data  = 0;
    free(index); index = 0;
    throw  OutOfMemoryException(__FILE__, __LINE__);
  }

  // Start Symbol Rule
  index[0] = data;
  data[0] = 0; // Rule Number
  data[1] = 0; // Precedence
  data[2] = 1; // Length of Right-Hand Side
  data[3] = 0; // Left-Hand Side (Augmented Start Symbol)
  // data[4] - reserved: Actual Start Symbol
  data[5] = 0; // End-of-Rule

  data[6] = -1;      // End-of-List
  eol     = data+6;  // always points to EOL
}
RulePool::~RulePool() { 
  free(data);
  free(index);
}

void RulePool::grow() {
  sshort *const  old = data;
  data = (sshort*)realloc(data, (dcap+=dinc)*sizeof(sshort));
  if(data == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  if(data != old) {
    size_t const  d = data-old;
    for(unsigned i = 0; i < nrno; index[i++] += d);
    eol += d;
  }
}

void RulePool::startSymbol(Symbol var) {
  assert(var.isVariable());
  data[4] = var;
}

void RulePool::startRule(Symbol var) {
  assert(var.isVariable());

  if(eol+5 >= data+dcap)  grow();
  sshort *const  ptr = eol;
  // ptr[0] - reserved: Rule Number
  // ptr[1] - reserved: Precedence
  ptr[2] = 0;   // length of right-hand side
  ptr[3] = var; // left-hand side
  // ptr[4] - reserved: End-of-Rule
  // ptr[5] - reserved: End-of-List
}

void RulePool::appendRule(Symbol sym) {
  assert(!sym.isEpsilon());
  assert(eol + eol[2] + 5 < data+dcap);

  sshort *ptr = eol + 2;  // point to current length of rhs
  ptr += ++*ptr + 1;      // inc length, point to next (reserved!) position
  *ptr = sym;             // append symbol
  if(ptr+2 >= data+dcap)  grow();  // reserve one more slot
}
  
unsigned short RulePool::finishRule(unsigned short prec) {
  assert(eol + eol[2] + 5 < data+dcap);

  sshort *ptr = eol;
  if(nrno >= icap) {
    index = (sshort**)realloc(index, (icap += iinc)*sizeof(sshort*));
    if(index == 0)  throw OutOfMemoryException(__FILE__, __LINE__);
  }
  index[nrno] = ptr;

  ptr[0] = nrno;   // assign rule number
  ptr[1] = prec;   // assign precedence
  ptr   += ptr[2]+4;
  *ptr++ = 0;      // End-of-Rule

  *(eol = ptr) = -1; // End-of-List

  return  nrno++;  
}

//+ Debugging / Visualization ++++++++++++++++++++++++++++++++++++++++++++++++
void RulePool::dump(std::ostream& o) const {
  for(Iterator  it(iterator(0));
      it.hasNext();
      o << it.next().toString(&terms, &vars) << std::endl);
}
