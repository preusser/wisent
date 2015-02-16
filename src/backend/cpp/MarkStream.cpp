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

#include "MarkStream.hpp"
#include <string.h>

using namespace wisent::backend::cpp;

//= LineCountBuf =============================================================

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
LineCountBuf::LineCountBuf() : lineno(1) {
  setp(buf, buf + BUF_SIZE-1);
}
LineCountBuf::~LineCountBuf() {
  sync();
}

void LineCountBuf::open(char const *file) {
  out.open(file, std::ios_base::out|std::ios_base::trunc);
}  

//+ std::filebuf interface +++++++++++++++++++++++++++++++++++++++++++++++++++
int LineCountBuf::overflow(int c) {
  char const *const  base = pbase();
  char       *const  ende = pptr();
  unsigned           len  = ende - base;
  
  // Count newlines
  *ende = '\0';
  for(char const *p = base; (p = strchr(p, '\n')) != 0; lineno++, p++);
  if(c != EOF) {
    *ende = c;
    if(c == '\n')  lineno++;
    len++;
  }

  // Forward to filebuf
  if(len > 0) {
    if(out.sputn(base, len) == EOF)  return  EOF;
    setp(buf, buf + BUF_SIZE-1);
  }
  return ~EOF;
}

int LineCountBuf::sync() {
  return  overflow(EOF);
}

//= MarkStream ===============================================================

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
MarkStream::MarkStream()
 : std::basic_ostream<char, std::char_traits<char> >(new LineCountBuf()), 
   out(static_cast<LineCountBuf*>(rdbuf())) {}
MarkStream::MarkStream(std::string const& _file)
 : std::basic_ostream<char, std::char_traits<char> >(new LineCountBuf()), 
   out(static_cast<LineCountBuf*>(rdbuf())) {
  open(_file);
}
MarkStream::~MarkStream() {
  delete  out;
}

void MarkStream::open(std::string const& _file) {
  out->open(_file.c_str());
  file = _file;
}

void MarkStream::mark() {
  *this << "#line " << out->line() << " \"" << file << "\"\n";
}

void MarkStream::mark(std::string const& file, unsigned line) {
  *this << "#line " << line << " \"" << file << "\"\n";
}
