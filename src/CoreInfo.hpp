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

#ifndef WISENT_COREINFO_HPP_INCLUDED
#define WISENT_COREINFO_HPP_INCLUDED

#include "Symbol.hpp"
#include "Core.hpp"
#include "Item.hpp"
#include "ItemInfo.hpp"
#include <list>
#include <map>

namespace wisent {
  class CoreInfo {
    //+ Map Types ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    typedef std::pair<Core const, CoreInfo>  CoreTuple;
  private:
    typedef std::map <Symbol, CoreTuple*>  GotoMap;
    typedef std::map <Item,   ItemInfo>    InfoMap;

    //+ Public Types for Access to GotoMap +++++++++++++++++++++++++++++++++++
  public:
    //- Single Entry ---------------------------------------------------------
    class Goto {
      GotoMap::iterator const  ptr;

    public:
      Goto(GotoMap::iterator _ptr) : ptr(_ptr) {}
      ~Goto() {}

    public:
      Symbol     on()     const { return  ptr->first; }
      CoreTuple& target() const { return *ptr->second; }
    };
    //- Iteration ------------------------------------------------------------
    class GotoIterator {
      GotoMap const&     par;
      GotoMap::iterator  pos;

    public:
      GotoIterator(GotoMap const& p)
	: par(p), pos(const_cast<GotoMap&>(p).begin()) {}
      ~GotoIterator() {}

    public:
      bool hasNext() { return  GotoMap::const_iterator(pos) != par.end(); }
      Goto next()    { return  pos++; }
    };

    //+ Data Members +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  private:
    unsigned const  index;
    unsigned const  lalen;
    GotoMap  gotos;
    InfoMap  infos;

  public:
    typedef std::pair<unsigned short, Symbol>  Reduction;
    typedef std::set<Reduction>  ReductionList;
  private:
    ReductionList  reduces;
    
    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    CoreInfo(unsigned index, unsigned lalen);
    ~CoreInfo();

  public:
    unsigned getIndex() const { return  index; }

    //+ Data Access ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void addGoto(Symbol sym, CoreTuple *core);
    Goto getGoto(Symbol sym);
    ItemInfo      & getInfo(Item const& i);
    ItemInfo const& getInfo(Item const& i) const;

  public:
    GotoIterator gotoIterator() const { return  gotos; }

  public:
    void addReduction(unsigned short rno, Symbol sym);
    ReductionList const& getReductions() const { return  reduces; }
    ReductionList      & getReductions()       { return  reduces; }

    //+ Lookahead Propagation ++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void propagate();
  };
}
#endif
