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

#ifndef WISENT_TERMPOOL_HPP_INCLUDED
#define WISENT_TERMPOOL_HPP_INCLUDED

#include "Terminal.hpp"
#include <iostream>

namespace wisent {
  class Context;

  class TermPool {
    //+ Data Members +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    IdentPool const& idents;

  private:
    /** next internal terminal number */
    ushort  nino;
    /** next external terminal number */
    ushort  neno;

    /** increment (in Terminals) for growth of insufficient buffer */
    enum { inc = 32 };
    /** current capacity (in Terminals) of data buffer */
    unsigned  cap;
    /** storage for all terminals */
    ushort *data;

    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    TermPool(IdentPool const& _idents);
    ~TermPool();
  private: // not to be implemented
    TermPool(TermPool const& o);
    TermPool& operator=(TermPool const& o);

    //+ inline - Helpers +++++++++++++++++++++++++++++++++++++++++++++++++++++
  private:
    /** Calculates the storage necessary for n Terminals. */
    static inline unsigned storage(unsigned n) { return  4*n + 1; }
    /**
     * Calculates the offset into the data buffer for the Terminal with
     * the internal representation ino. 
     */
    static inline unsigned index(ushort ino)   { return  4*(ino-1); }

    //+ Data Buffer Growth +++++++++++++++++++++++++++++++++++++++++++++++++++
  private:
    void grow();

    //+ Definition of Terminals ++++++++++++++++++++++++++++++++++++++++++++++
  public:
    Symbol defineLiteral(unsigned char lit, ushort prec = 0, 
			 Terminal::Assoc assoc = Terminal::NONE);
    Symbol defineToken(ushort ident, ushort prec, Terminal::Assoc assoc);

    //+ Size ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    unsigned size() const { return  nino-1; }
    unsigned lastExternal() const { return  neno-1; }

    //+ Access to Terminals +++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    //- Single Terminal ------------------------------------------------------
    Terminal get(ushort ino) const { return  Terminal(data + index(ino)); }
    Terminal get(Symbol sym) const { return  Terminal(data + index(sym)); }
    std::string toString(ushort ino, unsigned width = 0) const {
      return  get(ino).toString(&idents, width);
    }
    std::string toString(Symbol sym, unsigned width = 0) const {
      return  get(sym).toString(&idents, width);
    }
    std::string toString(Terminal term, unsigned width = 0) const {
      return  term.toString(&idents, width);
    }
    
    //- Iteration ------------------------------------------------------------
    class Iterator {
      ushort const *ptr;

    public:
      Iterator(ushort const *p) : ptr(p) {}
      ~Iterator() {}

    public:
      operator bool() const { return  hasNext(); }
      Terminal const  operator* () const { return  Terminal(ptr); }
      Terminal const *operator->() const {
	return  reinterpret_cast<Terminal const*>(this);
      }
      Iterator& operator++()    { ptr += 4;  return *this; }
      Iterator  operator++(int) { Iterator  res(ptr);  ptr += 4;  return  res; }

    public:
      bool hasNext() const { return *ptr != 0; }
      Terminal next() { Terminal  res(ptr);  ptr += 4;  return  res; }
    };
    Iterator iterator() const { return  Iterator(data); }

    //+ Debugging ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void dump(std::ostream& o) const;
  };
}
#endif
