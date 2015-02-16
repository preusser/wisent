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

#ifndef WISENT_VARPOOL_HPP_INCLUDED
#define WISENT_VARPOOL_HPP_INCLUDED

#include "types.hpp"
#include "Variable.hpp"
#include <iostream>

namespace wisent {
  class VarPool {
    //+ Data Members +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    IdentPool const& idents;

  private:
    /** next available variable number */
    sshort  nvno;

    /** increment (in Variables) for growth of insufficient buffer */
    enum { inc = 64 };
    /** current capacity (in Variables) of data buffer */
    unsigned  cap;
    /** storage for all variables */
    sshort *data;

    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    VarPool(IdentPool const& _idents);
    ~VarPool();
  private: // not to be implemented
    VarPool(VarPool const& o);
    VarPool& operator=(VarPool const& o);

    //+ inline - Helpers +++++++++++++++++++++++++++++++++++++++++++++++++++++
  private:
    /** Calculates the storage necessary for n Variables. */
    static inline unsigned storage(sshort n) { return  2*n + 1; }
    /** Calculates the offset into the data buffer for the Variable vno. */
    static inline unsigned index(sshort vno)   { return  2*~vno; }

    //+ Definition of Variables ++++++++++++++++++++++++++++++++++++++++++++++
  public:
    Symbol defineVariable(sshort ident);

    //+ Size ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    unsigned size() const { return ~nvno; }

    //+ Access to Variables +++++++++++++++++++++++++++++++++++++++++++++++++

    //- Single Variable ------------------------------------------------------
  public:
    Variable get(sshort vno) const { return  Variable(data + index(vno)); }
    std::string toString(sshort vno, unsigned width = 0) const {
      return  get(vno).toString(&idents, width);
    }

    //- Iteration ------------------------------------------------------------
    class Iterator {
      sshort const *ptr;

    public:
      Iterator(sshort const *p) : ptr(p) {}
      ~Iterator() {}

    public:
      operator bool() const { return  hasNext(); }
      Variable const  operator* () const { return  Variable(ptr); }
      Variable const *operator->() const {
        return  reinterpret_cast<Variable const*>(this);
      }
      Iterator& operator++()    { ptr += 2;  return *this; }
      Iterator  operator++(int) { Iterator  res(ptr);  ptr += 2;  return  res; }
    public:
      bool hasNext() const { return *ptr != 0; }
      Variable next() { Variable  res(ptr);  ptr += 2;  return  res; }
    };
    Iterator iterator() const { return  Iterator(data); }
  
    //+ Debugging ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void dump(std::ostream& o) const;
  };
}
#endif
