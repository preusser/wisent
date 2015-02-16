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

#ifndef WISENT_IMPLEMENTATIONPOOL_HPP_INCLUDED
#define WISENT_IMPLEMENTATIONPOOL_HPP_INCLUDED

#include "IDKeyedPtrMap.hpp"

namespace wisent {
  class Properties;

  template<class T>
  class ImplementationPool {
  public:
    class BaseEntry {
      char const *const  name;
      char const *const  desc;

    protected:
      BaseEntry(char const *_name, char const *_desc)
	: name(_name), desc(_desc) {
	ImplementationPool::registrate(this);
      }
      ~BaseEntry() {}

    public:
      char const *getName()        const { return  name; }
      char const *getDescription() const { return  desc; }

    public:
      virtual T* create(Properties const& props) const = 0;
    };

    template<class C>
    class Entry : public BaseEntry {
    public:
      Entry(char const *name, char const *desc)
	: BaseEntry(name, desc) {}
      ~Entry() {}

    public:
      T* create(Properties const& props) const { return  new C(props); };
    };

  private:
    typedef IDKeyedPtrMap<BaseEntry const>  EntryMap;
    static EntryMap& entries() {
      static EntryMap  map;
      return  map;
    }

  private:
    ImplementationPool();
    ~ImplementationPool();

  public:
    static BaseEntry const *registrate(BaseEntry const *entry) {
      return  entries().insert(entry->getName(), entry);
    }

    static T *create(char const *name, Properties const& props) {
      BaseEntry const *const  entry = entries().find(name);
      return (entry == 0)? 0 : entry->create(props);
    }

    static void dump(std::ostream& o) {
      typename EntryMap::Iterator  it = entries().iterator();
      while(it.hasNext()) {
	BaseEntry const *const  entry = it.next().second;
	o << entry->getName() << ":\t" << entry->getDescription() << '\n';
      }
    }
  };
}
#endif
