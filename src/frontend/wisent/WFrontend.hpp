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

#ifndef WISENT_FRONTEND_WISENT_FRONTEND_HPP_INCLUDED
#define WISENT_FRONTEND_WISENT_FRONTEND_HPP_INCLUDED

#include "../../Frontend.hpp"
#include "../../ImplementationPool.hpp"

namespace wisent {
  class Properties;

  namespace frontend {
    namespace wisent {
      class WFrontend : public Frontend {
	std::string const  name;

      public:
	WFrontend(Properties const& props);
	~WFrontend();

      public:
	void build(Grammar::Builder& bld);

      private:
	static ImplementationPool<Frontend>::Entry<WFrontend>  registrar;
      };
    }
  }
}
#endif
