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

#ifndef WISENT_GRAMMAR_HPP_INCLUDED
#define WISENT_GRAMMAR_HPP_INCLUDED

#include "Klass.hpp"
#include "Blocks.hpp"

#include "IdentPool.hpp"
#include "TermPool.hpp"
#include "VarPool.hpp"
#include "RulePool.hpp"
#include "ActionPool.hpp"

#include "ActionTable.hpp"

namespace wisent {
  class Frontend;

  /**
   * A Grammar holds the complete representation of a parsed grammar
   * specification and the analysis result describing a suitable
   * Shift/Reduce parser. It is the data structure to be inspected
   * by Backends to generate an implementation of this Shift/Reduce
   * parser in their target languages.
   */
  class Grammar {
  private:
    // Context data
    Klass       parsclass;    // class of parser to be generated
    Klass       svalclass;    // class of semantic value
    Blocks      blocks;

    // Actual grammar data
    IdentPool  idents;        // identifiers: ushort -> string
    TermPool   terms;         // terminals: +short [uses idents]
    VarPool    vars;          // variables: -short [uses idents]
    RulePool   rules;         // rules             [uses terms & vars]
    ActionPool actions;       // rule -> Block

    // Calculated table
    ActionTable  table;

  public:
    //- Construction of Grammar instances ------------------------------------
    class Builder {
      Grammar& par;

      //- Construction / Destruction ---
    private:
      Builder(Grammar& _par) : par(_par) {}
      ~Builder() {}
      friend class Grammar;

      //- Context Data ---
    public:
      void setParserClass(Klass const& k) { par.parsclass = k; }
      void setSValueClass(Klass const& s) { par.svalclass = s; }
      void addBlock(Blocks::Kind kind, Block const& block) {
	par.blocks.add(kind, block);
      }

      //- Grammar Definition --
    public:
      Symbol defineLiteral(char lit, ushort prec, Terminal::Assoc assoc) {
	return  par.terms.defineLiteral(lit, prec, assoc);
      }
      Symbol defineToken(std::string const& tok,
			 ushort prec, Terminal::Assoc assoc) {
	return  par.terms.defineToken(par.idents.add(tok.c_str()), prec, assoc);
      }
      ushort getPrecedence(Symbol s) const {
	return  par.terms.get(s).prec();
      }

      Symbol defineVariable(std::string const& var) { 
	return  par.vars.defineVariable(par.idents.add(var.c_str()));
      }

      void startSymbol(Symbol var)  { par.rules.startSymbol(var); }
      void startRule  (Symbol var)  { par.rules.startRule(var); }
      void appendRule (Symbol sym)  { par.rules.appendRule(sym); }
      void finishRule (ushort prec, Block const& action) {
	ushort const  rno = par.rules.finishRule(prec);
	par.actions.set(rno, action);
      }
    };
    
    //+ Constructor / Destructor +++++++++++++++++++++++++++++++++++++++++++++
  public:
    /** 
     * Will fully construct a Grammar object by executing the provided Source
     * on a temporary Builder instance.
     */
    Grammar(Frontend& src);
    ~Grammar();

    //+ Checking +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    /** Echos the internal representation to the specified ostream. */
    void echo(std::ostream& o) const;
    /**
     * Performs sanity checks as symbol usage and generates warnings on
     * the specified ostream.
     */
    void warn(std::ostream& o) const;

    /** Constructs the parser tables. Table information is streamed to o. */
    void analyze(std::ostream& o);

    //+ Backend Inspection +++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    // Context data
    Klass const& getParserClass() const { return  parsclass; }
    Klass const& getSValueClass() const { return  svalclass; }
    Block const& getBlock(Blocks::Kind kind) const {
      return  blocks.get(kind);
    }

    // Actual grammar data
    IdentPool  const& getIdents()  const { return  idents; }
    TermPool   const& getTerms()   const { return  terms; }
    VarPool    const& getVars()    const { return  vars; }
    RulePool   const& getRules()   const { return  rules; }
    ActionPool const& getActions() const { return  actions; }

    // Parse table
    ActionTable const& getParserTable() const { return  table; }
  };
}
#endif
