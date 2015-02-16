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

#ifndef WISENT_MARKSTREAM_HPP_INCLUDED
#define WISENT_MARKSTREAM_HPP_INCLUDED

#include <fstream>

namespace wisent {
  class Block;

  namespace backend {
    namespace cpp {
      /**
       * Implements a streambuf counting the lines output so far.
       */
      class LineCountBuf : public std::streambuf {
	unsigned  lineno;

	enum { BUF_SIZE = 400 };
	char                   buf[BUF_SIZE];
	std::filebuf           out;
	
      public:
	LineCountBuf();
	~LineCountBuf();
	
      public:
	void open(char const *file);
	unsigned line() {
	  sync();
	  return  lineno;
	}
	
      protected:
	int overflow(int c);
	int sync();
      };
      
      /**
       * Implements an ostream writing to a file, which will upon a call
       * to mark() insert a CPP line directive about the current position
       * in the output file.
       */
      class MarkStream : public std::ostream {
	LineCountBuf *out;
	std::string   file;
	
      public:
	MarkStream();
	MarkStream(std::string const& file);
	~MarkStream();
	
      public:
	void open(std::string const& file);
	void mark();
	void mark(std::string const& file, unsigned line);
      };
    }
  }
}
#endif
