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

#ifndef WISENT_FRONTEND_HPP_INCLUDED
#define WISENT_FRONTEND_HPP_INCLUDED

#include "Grammar.hpp"

namespace wisent {
  /**
   * Interface for a grammar generating frontend.
   */ 
  class Frontend {
  protected:
    Frontend() {}
  public:
    virtual ~Frontend() {}

  public:
    /** Build grammar via given Builder. */
    virtual void build(Grammar::Builder& bld) = 0;
  };
}
#endif
