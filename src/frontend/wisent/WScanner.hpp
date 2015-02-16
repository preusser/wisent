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

#ifndef WISENT_WSCANNER_HPP_INCLUDED
#define WISENT_WSCANNER_HPP_INCLUDED
#ifndef _SKIP_YYFLEXLEXER_
#include <FlexLexer.h>
#endif

#include <string>

namespace wisent {
  class WScanner : private yyFlexLexer {
    unsigned     line;     // current line number in source
    unsigned     mark;     // marked  line number in source
    std::string  text;     // identifiers, strings or errors
    unsigned     depth;    // depth of comments
    
  public:
    WScanner(std::istream& in);
    ~WScanner();
      
  public:
    int nextToken();
    
  public:
    unsigned    lineno()  const { return  line; }
    unsigned    getMark() const { return  mark; }
    std::string getText() const { return  text; }
  };
}
#endif
