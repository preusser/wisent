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

#ifndef WISENT_ITEM_HPP_INCLUDED
#define WISENT_ITEM_HPP_INCLUDED

#include <string>

namespace wisent {
  class RulePool;
  class IdentPool;
  class TermPool;
  class VarPool;

  class Item {
    /** rule number */
    unsigned short  rno;

    /** dot position */
    unsigned short  pos;

  public:
    Item(unsigned short _rno, unsigned short _dot)
      : rno(_rno), pos(_dot) {}
    ~Item() {}

  public:
    bool operator<(Item const& o) const {
      return  (rno < o.rno)? true  :
	      (o.rno < rno)? false : (pos < o.pos);
    }

  public:
    unsigned short rule() const { return  rno; }
    unsigned short dot () const { return  pos; }

  public:
    std::string toString(RulePool  const *rules  = 0) const;
  };
}
#endif
