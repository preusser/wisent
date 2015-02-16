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

#ifndef WISENT_COLLECTION_HPP_INCLUDED
#define WISENT_COLLECTION_HPP_INCLUDED

#include "Core.hpp"
#include "CoreInfo.hpp"

#include <map>

namespace wisent {
  class TermPool;
  class VarPool;
  class RulePool;
  class SymbolMatrix;
  class ActionTable;

  class Collection {
    // References to grammar members
    TermPool const& terms;
    VarPool  const& vars;
    RulePool const& rules;

    // Actual collection of cores
    typedef std::map<Core, CoreInfo>  CoreMap;
    CoreMap  cores;

  public:
    Collection(TermPool     const& terms,
	       VarPool      const& vars,
	       RulePool     const& rules,
	       SymbolMatrix const& reach,
	       SymbolMatrix const& first);
    ~Collection();

  private: // not to be implented
    Collection(Collection const&);
    Collection& operator=(Collection const& o);

    // Construction stages
  private:
    /** Calculates set of Cores without regarding lookaheads. */
    void construct(RulePool     const& rules,
		   unsigned            trmCnt, 
		   unsigned            varCnt,
		   SymbolMatrix const& reach);
    /** Annotates cores with immediate lookahead information. */
    void lookahead(RulePool     const& rules,
		   unsigned            trmCnt, 
		   unsigned            varCnt,
		   SymbolMatrix const& first);
    /** Propagates lookaheads through core, i.e. state, graph. */
    void propagate(RulePool     const& rules,
		   unsigned            trmCnt, 
		   unsigned            varCnt,
		   SymbolMatrix const& first);

  public:
    /**
     * Initializes the passed ActionTable according to the calculated
     * state graph.
     */
    void fill(ActionTable& table);

  private:
    /**
     * Inner class for the index-oriented sorting of cores for
     * more convenient dump.
     */
    class Order;
    friend class Order;
  public:
    void dump(std::ostream& o) const;
  };
}
#endif
