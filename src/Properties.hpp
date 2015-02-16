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

#ifndef WISENT_PROPERTIES_HPP_INCLUDED
#define WISENT_PROPERTIES_HPP_INCLUDED

#include "exceptions.hpp"
#include "IDKeyedPtrMap.hpp"

namespace wisent {
  /**
   * Implements a C-String-based name->value mapping. No ownership of the
   * strings is assumed.
   */
  class Properties {
    typedef IDKeyedPtrMap<char const>  Props;
    Props             entries;
    Properties const *fallback;

  public:
    Properties(Properties const *_fallback = 0) : fallback(_fallback) {}
    ~Properties() {}

  public:
    void setProperty(char const *name, char const *val) {
      entries.insert(name, val);
    }
    char const *getProperty (char const *name) const throw (NoEntryException);
    char const *findProperty(char const *name) const;

  public:
    Properties const *getFallback() const { return  fallback; }
    void setFallback(Properties const *_fallback) { fallback = _fallback; }

  public:
    void dump(std::ostream& o) const;
  };
}
#endif
