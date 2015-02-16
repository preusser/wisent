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

#ifndef WISENT_IDENTPOOL_HPP_INCLUDED
#define WISENT_IDENTPOOL_HPP_INCLUDED

#include <iostream>

namespace wisent {
  class IdentPool {
    enum { dinc = 0x200 };
    unsigned  dcap;
    char     *data;

    enum { iinc = 64 };
    unsigned  icap;
    char    **index;

    unsigned  nino;

  public:
    IdentPool();
    ~IdentPool();
  private:
    IdentPool(IdentPool const& o);
    IdentPool& operator=(IdentPool const& o);

  public:
    unsigned add(char const *ident);

  public:
    char const *get(unsigned ino) const { return  index[ino]; }

    class Iterator {
      char const *const *ptr;

    public:
      Iterator(char const *const *p) : ptr(p) {}
      ~Iterator() {}

    public:
      operator bool() const { return  hasNext(); }
      char const *operator*() const { return *ptr; }
      Iterator&   operator++()    { ptr++;  return *this; }
      Iterator    operator++(int) { Iterator  res(ptr++); return  res; }

    public:
      bool hasNext() const { return *ptr != 0; }
      char const *next() { return *ptr++; }
    };

    Iterator iterator() const { return Iterator(index+1); }

    void dump(std::ostream& o) const;
  };
};
#endif
