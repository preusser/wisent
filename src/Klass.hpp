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

#ifndef WISENT_KLASS_HPP_INCLUDED
#define WISENT_KLASS_HPP_INCLUDED

#include <string>
#include <vector>

namespace wisent {
  /**
   * Representation of a class identifier including its scope, i.e.
   * depending on the terminology of the backend its namespace, 
   * package or the like.
   */
  class Klass {
    std::string               name;
    std::vector<std::string>  spaces;
    std::string               adjunct;

  public:
    Klass();
    Klass(std::string const& klass, std::string const& adjunct = "");
    ~Klass();

  public:
    std::string const& getName()            const { return  name; }
    size_t             cntSpaces()          const { return  spaces.size(); }
    std::string const& getSpace(size_t idx) const { return  spaces[idx]; }
    std::string const& getAdjunct()         const { return  adjunct; }

  public:
    std::string getScope(std::string const& delim = "::") const;
    std::string toString(std::string const& delim = "::") const;
  };
}
#endif
