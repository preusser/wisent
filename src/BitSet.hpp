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

#ifndef WISENT_BITSET_HPP_INCLUDED
#define WISENT_BITSET_HPP_INCLUDED

namespace wisent {
  class BitMatrix;
  class SymbolMatrix;

  class BitSet {
    //+ Constants ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    typedef unsigned  chunk;
    enum { CHUNK_LEN = 8*sizeof(chunk) };
    static unsigned clen(unsigned blen) { return (blen-1)/CHUNK_LEN + 1; }

    //+ Instance Members +++++++++++++++++++++++++++++++++++++++++++++++++++++
    /** Length of BitSet. If negative, the data area is not owned. */
    signed const  len;
    chunk *const  dat;

    //+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++
  public:
    BitSet(unsigned len);
    ~BitSet();

  private:
    BitSet(signed _len, chunk *_dat) : len(_len), dat(_dat) {}
    friend class BitMatrix;

  protected:
    BitSet(BitSet const& o) : len((o.len > 0)? -o.len : o.len), dat(o.dat) {}
    friend class SymbolMatrix;

    //+ Length +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    unsigned length() const { return (len > 0)? len : -len; }

    //+ Clearing +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    void clear();
    bool isEmpty() const;

    //+ Element Access +++++++++++++++++++++++++++++++++++++++++++++++++++++++
  protected:
    class BRef {
      chunk *const  ptr;
      chunk  const  msk;

    public:
      BRef(chunk *_ptr, unsigned _ofs) : ptr(_ptr), msk(1<<_ofs) {}
      ~BRef() {}

    public:
      operator bool() const { return (*ptr&msk) != 0; }
      BRef& operator=(bool val) {
	if(val) *ptr |= msk; else *ptr &= ~msk;
	return *this;
      }
      BRef& operator=(BRef const& val) { return  operator=((bool)val); }
    };

  public:
    BRef       operator[](unsigned bit)      {
      return  BRef(dat+bit/CHUNK_LEN, bit%CHUNK_LEN);
    }
    BRef const operator[](unsigned bit) const {
      return  BRef(dat+bit/CHUNK_LEN, bit%CHUNK_LEN);
    }

  public:
    //+ Set Operations +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    BitSet& unify  (BitSet const& o);  // Union: A u  B
    BitSet& unifyNZ(BitSet const& o);  // Union: A u (B \ {0})
    
    bool subset(BitSet const& o) const;
    bool operator<=(BitSet const& o) const { return  subset(o); }
  };
}
#endif
