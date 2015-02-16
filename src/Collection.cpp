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

#include "Collection.hpp"

#include "TermPool.hpp"
#include "VarPool.hpp"
#include "RulePool.hpp"
#include "SymbolSet.hpp"
#include "SymbolMatrix.hpp"

#include "LR1Item.hpp"
#include "ItemInfo.hpp"
#include "ActionTable.hpp"

#include <stack>

#undef NDEBUG
#include <assert.h>

using namespace wisent;

Collection::Collection(TermPool     const& _terms,
		       VarPool      const& _vars,
		       RulePool     const& _rules,
		       SymbolMatrix const& _reach,
		       SymbolMatrix const& _first)
  : terms(_terms), vars(_vars), rules(_rules) {
  unsigned const  trmCnt = terms.size();
  unsigned const  varCnt = vars .size();

  construct(_rules, trmCnt, varCnt, _reach);
  lookahead(_rules, trmCnt, varCnt, _first);
  propagate(_rules, trmCnt, varCnt, _first);
}
Collection::~Collection() {}

void Collection::construct(RulePool     const& rules,
			   unsigned            trmCnt, 
			   unsigned            varCnt,
			   SymbolMatrix const& reach) {
  unsigned const  lalen = 1 + trmCnt;
  unsigned const  syms  = lalen + varCnt;

  assert(cores.empty());

  // Start with rule $ -> . S
  typedef CoreMap::value_type  CoreTuple;
  std::stack<CoreTuple*>  todo; {
    Core  init;
    init.add(Item(0, 0));
    todo.push(&*cores.insert(CoreTuple(init, CoreInfo(0, lalen))).first);
  }

  // Calculate cores of gotos
  {
    SymbolSet  cand(syms);  // candidates for goto(X)
  
    while(!todo.empty()) {
      CoreTuple  *curr = todo.top(); todo.pop();
      Core const& core = curr->first;
      CoreInfo&   info = curr->second;

      // Determine all grammar Symbols, for which goto is not empty
      cand.clear();
      for(Core::iterator it = core.begin(); it != core.end(); it++) {
	Rule   const  rule = rules.get(it->rule());
	Symbol const  sym  = rule.rhs(it->dot());
	if(!sym.isEpsilon()) {
	  if(sym.isVariable())  cand.unifyNZ(reach[sym]);
	  else  cand[sym] = true;
	}
      }

      // Calculate all gotos for all candidates
      for(signed  i = -varCnt; i <= (signed)trmCnt; i++) {
	if(cand[i]) {
	  Core  res;

	  // Scan all elements of the kernel
	  for(Core::iterator it = core.begin(); it != core.end(); it++) {
	    Symbol const  sym  = rules.get(it->rule()).rhs(it->dot());

	    // Just moving the dot by a single position
	    if(sym == i)  res.add(Item(it->rule(), it->dot()+1));

	    // Find gotos of non-kernel members
	    if(sym.isVariable() && reach[sym][i]) {
	      RulePool::Iterator  rit = rules.iterator();
	      while(rit.hasNext()) {
		Rule const  rule = rit.next();
		if((rule.rhs(0) == i) && reach[sym][rule.lhs()]) {
		  res.add(Item(rule.number(), 1));
		}
	      }
	    }
	  }

	  // Register if a new core has been found
	  CoreMap::iterator  jt = cores.find(res);
	  if(jt == cores.end()) {
	    unsigned const  index = cores.size();
	    jt = cores.insert(CoreTuple(res, CoreInfo(index, lalen))).first;
	    todo.push(&*jt);
	  }
	  info.addGoto(i, &*jt);
	}
      }
    }
  }
}

void Collection::lookahead(RulePool     const& rules,
			   unsigned            trmCnt, 
			   unsigned            varCnt,
			   SymbolMatrix const& first) {
  SymbolSet  f(1+trmCnt+varCnt);  // Result of FIRST calculation

  // Calculate lookaheads for each kernel
  for(CoreMap::iterator  cm = cores.begin(); cm != cores.end();) {
    Core const& core = cm->first;
    CoreInfo  & info = cm++->second;

    // Lookahead discovery for each kernel item
    for(Core::iterator ct = core.begin(); ct != core.end();) {
      Item   const& item = *ct++;
      Symbol const  rod  = rules.get(item.rule()).rhs(item.dot());

      // Only bother with non-reduction items
      if(rod.isEpsilon())  info.getInfo(item);
      else {
	// Calculate closure for current core item
	typedef std::set<LR1Item>  LR1Set;
	LR1Set  closure;

	if(rod.isTerminal())  closure.insert(LR1Item(item, 0));
	else {
	  std::stack<LR1Item>  todo;    // newly added items not yet closed
	  todo.push(LR1Item(item, 0));  // EPS-lookahead signaling propagation
	  while(!todo.empty()) {
	    LR1Item  const& lr1  =*closure.insert(todo.top()).first; todo.pop();
	    Rule     const  rule = rules.get(lr1.rule());
	    unsigned const  dot  = lr1.dot();
	    Symbol   const  B    = rule.rhs(dot);
	    
	    // A -> u . B v, a
	    if(B.isVariable()) {
	      // calculate FIRST(v a)
	      rule.first(first, f, dot+1);
	      if(f[0]) {
		f[0]               = false;
		f[lr1.lookahead()] = true;
	      }
	      
	      // find all rules B -> w
	      for(RulePool::Iterator rt(rules.iterator()); rt.hasNext();) {
		Rule const  r = rt.next();
		if(r.lhs() == B) {
		  // add closure item for each terminal from FIRST(v a)
		  for(unsigned b = 0; b <= trmCnt; b++) if(f[b]) {
		    LR1Item const  ni(Item(r.number(), 0), b);
		    if(closure.find(ni) == closure.end())  todo.push(ni);
		  }
		}
	      }
	    }
	  }
	}
	// closure contains closure of { [item, 0] }

	// Iterate through closure to determine lookaheads
	ItemInfo& iinfo = info.getInfo(item);
	for(LR1Set::iterator  st = closure.begin(); st != closure.end();) {
	  LR1Item const& lr1 =*st++;  // lr1: A -> u . X v, a
	  Symbol  const  X   = rules.get(lr1.rule()).rhs(lr1.dot());

          // Is epsilon reduction for X required?
	  if(X.isEpsilon()) {
	    info.addReduction(lr1.rule(), lr1.lookahead());
	  }
	  else {
	    // determine ItemInfo for A -> u X . v in GOTO(X)
	    CoreInfo& go2core = info.getGoto(X).target().second;
	    ItemInfo& go2item = go2core.getInfo(Item(lr1.rule(), lr1.dot()+1));
	    
	    Symbol const  la = lr1.lookahead();
	    if(la == 0)  iinfo.addForward(&go2item);   // propagated lookahead
	    else  go2item.addLookahead(la);            // spontaneous lookahead
	  }
	}
      } // if non-reduction item
    } // for all kernel items
  } // for all kernels

  // Spontaneous lookahead for $ -> . S
  cores.begin()->second.getInfo(Item(0, 0)).addLookahead(0);

} // lookahead()

void Collection::propagate(RulePool     const& rules,
			   unsigned            trmCnt, 
			   unsigned            varCnt,
			   SymbolMatrix const& first) {

  // request each kernel to propagate its lookaheads
  for(CoreMap::iterator cn = cores.begin(); 
      cn != cores.end(); 
      cn++->second.propagate());

  // append propagated lookaheads to epsilon reductions on $
  SymbolSet  f(1+trmCnt+varCnt);
  for(CoreMap::iterator  cm = cores.begin(); cm != cores.end();) {
    Core const& core = cm->first;
    CoreInfo  & info = cm++->second;

    CoreInfo::ReductionList& reduces = info.getReductions();
    for(CoreInfo::ReductionList::iterator rt = reduces.begin();
        rt != reduces.end();) {
      // Check for C -> ; $  (no spontaneous lookahead)
      if(rt->second.isEpsilon()) {
        unsigned short const  rno = rt->first;
        Symbol         const  lhs = rules.get(rno).lhs();

	// Delete reduction in favor of those with propagated lookaheads
	{
	  CoreInfo::ReductionList::iterator  rm = rt++;
	  reduces.erase(rm);
	}

        // Check every member whose propagates we might need to consider
        for(Core::iterator  ct = core.begin(); ct != core.end();) {
          Item           const& item = *ct++;  // A -> u . B v
          Rule           const  rule = rules.get(item.rule());
          unsigned short const  dot  = item.dot();

          if(first[rule.rhs(dot)][lhs]) { // B => C x
            rule.first(first, f, dot);
            if(f[0]) { // B v => ;
              // Consider all lookaheads of item
              SymbolSet const& las = info.getInfo(item).getLookaheads();
              for(unsigned la = 0; la <= trmCnt; la++) if(las[la]) {
                reduces.insert(CoreInfo::Reduction(rno, la));
              }
            }
          }
        }
      }
      else  rt++;
    }
  }
}

//+ Calculation of ActionTable +++++++++++++++++++++++++++++++++++++++++++++++
void Collection::fill(ActionTable& table) {
  table.reset(cores.size(), terms.size(), vars.size());
  ActionTable::Builder  bld(table, terms, rules);

  for(CoreMap::iterator cm = cores.begin(); cm != cores.end();) {
    Core     const&   core = cm->first;
    CoreInfo const&   info = cm++->second;

    bld.selectRow(info.getIndex());

    // turn goto entries into shifts or real gotos
    for(CoreInfo::GotoIterator gt = info.gotoIterator(); gt.hasNext();) {
      CoreInfo::Goto go2(gt.next());

      Symbol   const  on = go2.on();
      unsigned const  to = go2.target().second.getIndex();

      assert(!on.isEpsilon());
      if(on.isTerminal())  bld.shift(on, to);
      else  bld.go2(on, to);
    }

    // heed epsilon reductions
    CoreInfo::ReductionList const& reduces = info.getReductions();
    for(CoreInfo::ReductionList::const_iterator rt = reduces.begin();
	rt != reduces.end(); rt++)  bld.reduce(rt->second, rt->first);

    // identify other reductions from kernel
    for(Core::iterator ct = core.begin(); ct != core.end();) {
      Item           const& item = *ct++;
      unsigned short const  rno  = item.rule();

      if(rules.get(rno).rhs(item.dot()).isEpsilon()) {
	SymbolSet const& la = info.getInfo(item).getLookaheads();
	for(unsigned i = la.length(); i-- > 0;) {
	  if(la[i])  bld.reduce(i, rno);
	}
      }
    }
  }
}

//+ Visualization ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Collection::Order {
  CoreMap::const_iterator  val;
  
public:
  Order() {}
  Order(CoreMap::const_iterator v) : val(v) {}
  ~Order() {}
  
public:
  bool operator<(Order const& o) const {
    return  val->second.getIndex() < o.val->second.getIndex();
  }
  CoreMap::value_type const& operator* () const { return  *val; }
  CoreMap::value_type const *operator->() const { return &*val; }
};

void Collection::dump(std::ostream& o) const {
  typedef std::set<Order>  OrderSet;
  OrderSet  order;

  for(CoreMap::const_iterator  cm = cores.begin(); cm != cores.end();) {
    order.insert(cm++);
  }

  for(OrderSet::const_iterator  cn = order.begin(); cn != order.end();) {
    Core     const& core = (*cn)->first;
    CoreInfo const& info = (*cn++)->second;

    // Print index
    o << info.getIndex() << ":\n";

    // Print core items
    for(Core::iterator ct = core.begin(); ct != core.end();) {
      Item      const& item =*ct++;
      SymbolSet const& las  = info.getInfo(item).getLookaheads();

      // item
      o << "  " << item.toString(&rules) << ';';

      // lookaheads
      if(las[0])  o << " $";
      signed const  l = las.length()-1;
      for(signed i = 1; i++ < l;)  if(las[i]) {
	o << ' ' << terms.toString(i);
      }
      o << '\n';  // item
    }

    // Print reductions
    CoreInfo::ReductionList const&  rl = info.getReductions();
    for(CoreInfo::ReductionList::const_iterator  rt = rl.begin();
	rt != rl.end(); rt++) {
      unsigned short const  rno = rt->first;
      Symbol         const  la  = rt->second;

      o << "RDC: " << rules.toString(rno);

      o << "; ";
      if(la.isEpsilon()) o << '$';
      else  o << terms.toString(la);
      o << '\n';
    }

    // Print gotos
    for(CoreInfo::GotoIterator gt = info.gotoIterator(); gt.hasNext();) {
      CoreInfo::Goto  go2(gt.next());
      Symbol const    on = go2.on();
      
      o << "on "  << on.toString(&terms, &vars, 4)
        << " => " << go2.target().second.getIndex() << '\n';
    }

    o << std::endl;
  }
}
