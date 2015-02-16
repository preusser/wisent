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

#ifndef WISENT_IDKEYEDPTRMAP_HPP_INCLUDED
#define WISENT_IDKEYEDPTRMAP_HPP_INCLUDED

#include <map>
#include <string.h>
#include <iostream>

namespace wisent {
  /**
   * Implements a non-shrinking map from C-String-IDs to pointers.
   * Ownership is claimed neither from the IDs nor the pointers. This
   * Map will never make an attempt of memory deallocation. Its sole
   * purpose is to provide the mapping.
   * The base IDKeyedPMap uses untyped void*-pointers. For type-safe
   * access use the template class IDKeyedPtrMap. Latter is backed
   * by the untyped IDKeyedPMap and only adds the inlined type-safe
   * interface usually not generating extra object code.
   */
  class IDKeyedPMap {
    class Comparator {
    public:
      bool operator()(const char* s1, const char* s2) const {
        return  strcmp(s1, s2) < 0;
      }
    };
    typedef std::map<char const*, void*, Comparator>  PMap;
    PMap  pmap;

  public:
    IDKeyedPMap();
    ~IDKeyedPMap();

  public:
    void *insert(char const *id, void *p);
    void *find  (char const *id) const;

  public:
    class Iterator {
      PMap::const_iterator  pos;
      PMap::const_iterator  end;

    public:
      Iterator(PMap const& _map);
      ~Iterator() {}

    public:
      bool  hasNext() const { return  pos != end; }
      std::pair<char const*, void*> next() { return *pos++; }
    };
    Iterator iterator() const { return  Iterator(pmap); }
  };

  template<class T>
  class IDKeyedPtrMap {
    IDKeyedPMap  pmap;

  public:
    IDKeyedPtrMap() {}
    ~IDKeyedPtrMap() {}

  public:
    T *insert(char const *id, T *p) {
      return  static_cast<T*>(pmap.insert(id,
           const_cast<void*>(static_cast<void const*>(p))));
    }
    T *find  (char const *id) const {
      return  static_cast<T*>(pmap.find(id));
    }

  public:
    class Iterator {
      IDKeyedPMap::Iterator  it;

    public:
      Iterator(IDKeyedPMap const& _map) : it(_map.iterator()) {}
      ~Iterator() {}

    public:
      bool  hasNext() const { return  it.hasNext(); }
      std::pair<char const*, T*> next() {
	std::pair<char const*, void*>  res = it.next();
	return  std::pair<char const*, T*>
	  (res.first, static_cast<T*>(res.second));
      }
      //  T *next() { return  static_cast<T*>(it.next()); }
    };
    Iterator iterator() const { return  Iterator(pmap); }
  };
}
#endif
