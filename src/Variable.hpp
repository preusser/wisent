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

#ifndef WISENT_VARIABLE_HPP_INCLUDED
#define WISENT_VARIABLE_HPP_INCLUDED

#include "Symbol.hpp"
#include <string>

namespace wisent {
  class IdentPool;

  /**
   * Defines the internal representation of a non-terminal symbol (variable),
   * The only member is a pointer into an array of signed shorts.
   * The interpretation is as follows:
   *   *(ptr + 0)  -  the internal symbol number (less than zero)
   *   *(ptr + 1)  -  the index of the variable name in the IdentPool
   */
  class Variable {
    typedef signed short  sshort;
    sshort const *const  ptr;
    
  public:
    Variable(sshort   const *p) : ptr(p) {}
    Variable(Variable const& o) : ptr(o.ptr) {}
    ~Variable() {}

  public:
    operator Symbol() const { return  Symbol(*ptr); }
    sshort internal() const { return  ptr[0]; }
    sshort ident()    const { return  ptr[1]; }
    
    std::string toString(IdentPool const *idents = 0, 
			 unsigned width = 0) const;
  };
}
#endif
