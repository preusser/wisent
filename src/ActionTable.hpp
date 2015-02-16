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

#ifndef WISENT_ACTIONTABLE_HPP_INCLUDED
#define WISENT_ACTIONTABLE_HPP_INCLUDED

#include "Symbol.hpp"

namespace wisent {
  class RulePool;

  /**
   * Represents the combined action and goto tables of the generated
   * shift/reduce-parser. Each line represents a state (core) of the
   * the parser, the initial state is numbered zero (0). Columns are
   * indexed by Symbols. Variables (negative) index the goto part,
   * Terminals (positive) and End-of-Input (zero) index the action
   * part. Shift actions are encoded by positive numbers determining
   * the successor state; reduce actions by negative numbers naming
   * the reduction rule. Zero (0) entries signify places without a defined
   * action, i.e. parse errors. One (1) entries signify acceptance of the
   * input, i.e. NO shift action.
   */
  class ActionTable {
    unsigned  height;
    unsigned  width;
    unsigned  offset;
    sshort   *data;

    //+ Construction +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    ActionTable() : height(0), width(0), offset(0), data(0) {}
    ~ActionTable() { delete [] data; }

    void reset(unsigned coreCnt, unsigned trmCnt, unsigned varCnt);

  public:
    unsigned cntRows()       const { return  height; }
    unsigned cntActionCols() const { return  width - offset; }
    unsigned cntGotoCols()   const { return  offset; }

    //+ Data Access ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  public:
    class Row_const;
    class Row {
      sshort *ptr;

    private:
      Row(sshort *_ptr) : ptr(_ptr) {}
      friend class ActionTable;
      friend class ActionTable::Row_const;
    public:
      ~Row() {}

    public:
      sshort& operator[](Symbol sym)       { return  ptr[sym]; }
      sshort  operator[](Symbol sym) const { return  ptr[sym]; }
    };
    class Row_const {
      sshort const *ptr;

    private:
      Row_const(sshort const *_ptr) : ptr(_ptr) {}
      friend class ActionTable;
    public:
      Row_const(Row const& o) : ptr(o.ptr) {}
      ~Row_const() {}

    public:
      sshort  operator[](Symbol sym) const { return  ptr[sym]; }
    };

    Row       operator[](unsigned core)       {
      return  data + (core*width + offset);
    }
    Row_const operator[](unsigned core) const {
      return  data + (core*width + offset);
    }

    //+ Higher Level Construction ++++++++++++++++++++++++++++++++++++++++++++
  public:
    class Builder {
      ActionTable   & table;
      TermPool const& terms;
      RulePool const& rules;

      ushort  core;
      Row     row;

    public:
      Builder(ActionTable   & _table,
	      TermPool const& _terms, RulePool const& _rules)
	: table(_table), terms(_terms), rules(_rules),
	  core(0), row(table[0]) {}
virtual
      ~Builder() {}

    public:
      void selectRow(ushort state);
      void shift (Symbol sym, ushort state);
      void reduce(Symbol sym, ushort rule);
      void go2   (Symbol sym, ushort state);

    private:
      typedef enum { SHIFT, REDUCE } SHIFT_REDUCE;
      SHIFT_REDUCE shift_reduce(Symbol sym, ushort rno) const;

    protected:
      virtual void 
      conflict(ushort state, Symbol sym, sshort win, sshort loose);
    };
  };
}
#endif
