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

#include "WBuilder.hpp"

using wisent::frontend::wisent::WBuilder;
using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
WBuilder::WBuilder(Grammar::Builder& _bld, std::string const& _file)
  : bld(_bld), file(_file), prec(0), assoc(Terminal::NONE), var(0) {}
WBuilder::~WBuilder() {}

//+ Operator Definition ++++++++++++++++++++++++++++++++++++++++++++++++++++++
void WBuilder::setAssoc(Terminal::Assoc _assoc) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::setAssoc('" << _assoc << "')" << std::endl;
#endif
  assoc = _assoc;
  prec++;
}

void WBuilder::defineTerminal(char lit) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::defineTerminal('" << lit << "')" << std::endl;
#endif
  litMap[lit] = bld.defineLiteral(lit, prec, assoc);
}

void WBuilder::defineTerminal(std::string const& tok) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::defineTerminal(" << tok << ')' << std::endl;
#endif
  idMap[tok] = bld.defineToken(tok, prec, assoc);
}

//+ Rule Definition ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void WBuilder::startSymbol(std::string const& var) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::startSymbol(" << var << ')' << std::endl;
#endif
  IdentIterator  it = idMap.find(var);
  if(it == idMap.end()) {
    it = idMap.insert(IdentMap::value_type(var, bld.defineVariable(var))).first;
  }
  else {
    if(!it->second.isVariable()) {
      throw BuildException(__FILE__, __LINE__,
			   "Redefinition of Terminal " + var);
    }
  }
  bld.startSymbol(it->second);
}

void WBuilder::startRule (std::string const& id) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::startRule(" << id << ')' << std::endl;
#endif
  IdentIterator  it = idMap.find(id);
  if(it == idMap.end()) {
    it = idMap.insert(IdentMap::value_type(id, bld.defineVariable(id))).first;
  }
  else {
    if(!it->second.isVariable()) {
      throw BuildException(__FILE__, __LINE__,
			   "Redefinition of Terminal " + id);
    }
  }
  bld.startRule(var = it->second);
  prec = 0;
}

void WBuilder::appendRule(char lit) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::appendRule('" << lit << "')" << std::endl;
#endif
  LiteralIterator  it = litMap.find(lit);
  
  if(it == litMap.end()) {
    it = litMap.insert(LiteralMap::value_type(lit, bld.defineLiteral(lit, 0, Terminal::NONE)))
               .first;
  }

  Symbol const  s = it->second;
  bld.appendRule(s);
  prec = bld.getPrecedence(s);
}

void WBuilder::appendRule(std::string const& ident) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::appendRule(" << ident << ')' << std::endl;
#endif
  IdentIterator  it = idMap.find(ident);

  if(it == idMap.end()) {
    it = idMap.insert(IdentMap::value_type(ident, bld.defineVariable(ident)))
              .first;
  }

  Symbol const  s = it->second;
  bld.appendRule(s);
  if(s.isTerminal())  prec = bld.getPrecedence(s);
}

void WBuilder::actionRule() {
#ifdef TRACE
  std::clog << "wisent::WBuilder::actionRule()" << std::endl;
#endif
  Block  action;
  action.file = file;
  bld.finishRule(prec, action);
}

void WBuilder::actionRule(Block& action) {
#ifdef TRACE
  std::clog << "wisent::WBuilder::actionRule(" << action.code << ')' << std::endl;
#endif
  action.file = file;
  bld.finishRule(prec, action);
}

void WBuilder::startAlternative() {
#ifdef TRACE
  std::clog << "wisent::WBuilder::startAlternativeRule()" << std::endl;
#endif
  bld.startRule(var);
}
