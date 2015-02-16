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

#ifndef WISENT_ACTIONPOOL_HPP_INCLUDED
#define WISENT_ACTIONPOOL_HPP_INCLUDED

#include "Block.hpp"
#include <vector>

namespace wisent {
  class ActionPool {
    typedef std::vector<Block>  ActList;
    ActList  acts;

    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    ActionPool();
    ~ActionPool();

    //+ Contents Acces +++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void set(unsigned short rno, Block const& action);

    size_t       size()                  const { return  acts.size(); }
    Block const& get(unsigned short rno) const { return  acts[rno]; }
  };
}
#endif
