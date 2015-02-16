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

#include "Terminal.hpp"

#include "IdentPool.hpp"
#include <sstream>

using namespace wisent;

std::string Terminal::toString(IdentPool const *idents, unsigned width) const {
  static char const  esc[] = "'\\?'";
  static char const  lit[] = "'?'";
  static char const  xno[] = "'\\xhh'";
  static char const  hex[] = "0123456789ABCDEF";
  
  if(isLiteral()) {
    unsigned const  c = external();
    std::string     res;
    
    switch(c) {
    case '\n':
      res = esc;
      res[2] = 'n';
      break;
    case '\t':
      res = esc;
      res[2] = 't';
      break;
    case '\v':
      res = esc;
      res[2] = 'v';
      break;
    case '\b':
      res = esc;
      res[2] = 'b';
      break;
    case '\r':
      res = esc;
      res[2] = 'r';
      break;
    case '\f':
      res = esc;
      res[2] = 'f';
      break;
    case '\a':
      res = esc;
      res[2] = 'a';
      break;
    case '\\':
      res = esc;
      res[2] = '\\';
      break;
    case '\'':
      res = esc;
      res[2] = '\'';
      break;
    case '\"':
      res = esc;
      res[2] = '\"';
      break;
    default:
      if((32 <= c) && (c <= 126)) {
	res = lit;
	res[1] = c;
      }
      else {
	res = xno;
	res[2] = hex[(c >> 4) & 0xF];
	res[3] = hex[(c >> 0) & 0xF];
      }
      break;
    }
    if(width)  res.resize(width, ' ');
    return  res;
  }
  else {
    if(idents) {
      std::string  res(idents->get(ident()));
      if(width)  res.resize(width, ' ');
      return  res;
    }
    else {
      std::ostringstream  res;
      res.setf(std::ios::right, std::ios::adjustfield);
      if(width)  res.width(width);
      res << external();
      return  res.str();
    }
  }
}
