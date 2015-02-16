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

#include "BitMatrix.hpp"
#include "exceptions.hpp"

using wisent::BitMatrix;

BitMatrix::BitMatrix(unsigned _rows, unsigned _cols)
  : rws(_rows), cls(_cols), dat(new chunk*[rws]) {

  if(!dat)  throw OutOfMemoryException(__FILE__, __LINE__);
  unsigned const  cl = clen(cls);
  chunk **p = dat+rws;
  while(p > dat) {
    chunk *row = *--p = new chunk[cl];
    if(!row) {
      while(++p < dat+rws)  delete [] *p;
      delete [] dat;
      throw OutOfMemoryException(__FILE__, __LINE__);
    }
    for(chunk *q = row+cl; q > row; *--q = 0);
  }
}

BitMatrix::~BitMatrix() {
  for(chunk **p = dat+rws; p > dat; delete [] *--p);
  delete [] dat;
}
