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

#ifndef WISENT_RULE_HPP_INCLUDED
#define WISENT_RULE_HPP_INCLUDED

#include "Symbol.hpp"
#include <string>

namespace wisent {
  class IdentPool;
  class TermPool;
  class VarPool;

  class SymbolSet;
  class SymbolMatrix;

  /**
   * Defines the internal representation of a grammar rule.
   * The only member is a pointer into an array of signed shorts.
   * The interpretation is as follows:
   *   *(ptr + 0)  -  rule number
   *   *(ptr + 1)  -  precedence
   *   *(ptr + 2)  -  rule length =: l
   *   *(ptr + 3)  -  left-hand side (derived variable)
   *   *(ptr + 4)  -  first symbol on right-hand side
   *     ...
   *   *(ptr+3+l)  -  last symbol on right-hand side
   *   *(ptr+4+l)  -  beyond the end of rule: zero (0)
   *
   * Note: All rules end in a zero (0), which is not counted toward
   *       the rule length. This zero is also interpreted as epsilon for
   *       epsilon derivations.
   */
  class Rule {
    typedef unsigned short  ushort;
    typedef   signed short  sshort;
    sshort const *const  ptr;

  public:
    Rule(sshort const *p) : ptr(p) {}
    Rule(Rule   const& o) : ptr(o.ptr) {}
    ~Rule() {}

  public:
    ushort number()        const { return  ptr[0]; }
    ushort prec()          const { return  ptr[1]; }
    ushort length()        const { return  ptr[2]; }
    Symbol lhs()           const { return  ptr[3]; }
    Symbol rhs(ushort idx) const { return  ptr[4+idx]; }
    
    void first(SymbolMatrix const& fm, SymbolSet& f, ushort idx = 0) const;

    std::string toString(TermPool  const *terms  =  0,
			 VarPool   const *vars   =  0,
			 unsigned  const  dot    = ~0u) const;
  };
}
#endif
