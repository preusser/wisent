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

#ifndef WISENT_BLOCKS_HPP_INCLUDED
#define WISENT_BLOCKS_HPP_INCLUDED

#include "Block.hpp"
#include "exceptions.hpp"

#include <map>

namespace wisent {
  class Blocks {
  public:
    typedef enum {
      HEADER_PREAMBLE       = 0,
      PARSER_INLINE         = 1,
      PARSER_IMPLEMENTATION = 2
    } Kind;

  private:
    typedef std::map<Kind, Block>   BlockMap;
    BlockMap  blocks;

  public:
    Blocks();
    ~Blocks();

  public:
    void add(Kind kind, Block const& block);
    Block const& get(Kind kind) const throw (NoEntryException);
    Block const *find(Kind kind) const;
  };
}
#endif
