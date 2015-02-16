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

#ifndef WISENT_SYMBOL_HPP_INCLUDED
#define WISENT_SYMBOL_HPP_INCLUDED

#include "types.hpp"
#include <string>

namespace wisent {
  class TermPool;
  class VarPool;

  class Symbol {
    sshort  no;

  public:
    Symbol(sshort _no = 0) : no(_no) {}
    ~Symbol() {}

  public:
    operator sshort() const { return  no; }
    sshort internal() const { return  no; }

  public:
    bool isVariable() const { return  no <  0; }
    bool isEpsilon()  const { return  no == 0; }
    bool isTerminal() const { return  no >  0; }

  public:
    std::string toString(TermPool  const *terms = 0,
			 VarPool   const *vars  = 0,
			 unsigned         width = 0) const;
  };
}
#endif
