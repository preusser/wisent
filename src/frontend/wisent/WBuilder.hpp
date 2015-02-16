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

#ifndef WISENT_FRONTEND_WISENT_WBUILDER_HPP_INCLUDED
#define WISENT_FRONTEND_WISENT_WBUILDER_HPP_INCLUDED

#include "../../Symbol.hpp"
#include "../../Terminal.hpp"
#include "../../Grammar.hpp"

#include <string>
#include <map>

namespace wisent {
  namespace frontend {
    namespace wisent {
      class WBuilder {
	typedef std::map<std::string, Symbol>  IdentMap;
	typedef std::map<char,        Symbol>  LiteralMap;
	typedef IdentMap  ::iterator  IdentIterator;
	typedef LiteralMap::iterator  LiteralIterator;

	Grammar::Builder & bld;
	std::string const  file;

	IdentMap    idMap;
	LiteralMap  litMap;

	unsigned short   prec;
	Terminal::Assoc  assoc;
	Symbol           var;

	//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++
      public:
	WBuilder(Grammar::Builder& bld, std::string const& file);
	~WBuilder();

	//+ Context Definition +++++++++++++++++++++++++++++++++++++++++++++++
      public:
	void setParserClass(Klass const& klass) {
	  bld.setParserClass(klass);
	}
	void setSValueClass(Klass const& klass) {
	  bld.setSValueClass(klass);
	}
	void setHeaderPreamble(Block& block) {
	  block.file = file;
	  bld.addBlock(Blocks::HEADER_PREAMBLE, block);
	}
	void setParserInline(Block& block) {
	  block.file = file;
	  bld.addBlock(Blocks::PARSER_INLINE, block);
	}
	void setParserImpl(Block& block) {
	  block.file = file;
	  bld.addBlock(Blocks::PARSER_IMPLEMENTATION, block);
	}

	//+ Terminal Definition ++++++++++++++++++++++++++++++++++++++++++++++
      public:
	void setAssoc(Terminal::Assoc assoc);
	void defineTerminal(char lit);
	void defineTerminal(std::string const& tok);

	//+ Grammar Defintion ++++++++++++++++++++++++++++++++++++++++++++++++
      public:
	void startSymbol(std::string const& var);
	void startRule  (std::string const& var);
	void appendRule (char lit);
	void appendRule (std::string const& ident);
	void actionRule ();
	void actionRule (Block& action);
	void startAlternative();
      };
    }
  }
}
#endif
