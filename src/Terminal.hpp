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

#ifndef WISENT_TERMINAL_HPP_INCLUDED
#define WISENT_TERMINAL_HPP_INCLUDED

#include "Symbol.hpp"
#include <string>

namespace wisent {
  class IdentPool;

  /**
   * Defines the internal representation of a terminal symbol.
   * The only member is a pointer into an array of unsigned shorts.
   * The interpretation is as follows:
   *   *(ptr + 0)  -  the internal symbol number (greater than zero)
   *   *(ptr + 1)  -  the external symbol number to be used by the scanner:
   *                   - the extended ASCII code for symbols up to (char)255
   *                   - an asigned number (>255) for named terminals
   *   *(ptr + 2)  -  in the case of a named terminal, the index of the name
   *                  into the IdentPool
   *   *(ptr + 3)  -  precedence and associativity ( (prec << 2) | assoc )
   */
  class Terminal {
  public:
    typedef enum { NONE = 0, RIGHT = 1, LEFT = 2 }  Assoc;

  private:
    typedef unsigned short  ushort;
    ushort const *const  ptr;
    
  public:
    Terminal(ushort   const *p) : ptr(p) {}
    Terminal(Terminal const& o) : ptr(o.ptr) {}
    ~Terminal() {}

  public:
    //- Low-Level Properies ------------------------------------------------
    operator Symbol() const { return  Symbol(*ptr); }
    ushort internal() const { return  ptr[0]; }
    ushort external() const { return  ptr[1]; }
    ushort ident()    const { return  ptr[2]; }
    ushort prec()     const { return  ptr[3] >> 2; }

    //- High-Level Properties ----------------------------------------------
    bool isLiteral() const { return  ptr[1] < 0x100; }
    
    bool isNone()  const { return (ptr[3]&3) == 0; }
    bool isRight() const { return (ptr[3]&1) != 0; }
    bool isLeft()  const { return (ptr[3]&2) != 0; }
    
    //- Visualization ------------------------------------------------------
    std::string toString(IdentPool const *idents = 0,
			 unsigned width = 0) const;
  };
}
#endif
