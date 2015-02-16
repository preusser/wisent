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

#include "JavaBackend.hpp"

#include "../../Properties.hpp"
#include "../../Grammar.hpp"

#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace wisent;
using namespace wisent::backend::java;

//+ Output of Code Blocks ++++++++++++++++++++++++++++++++++++++++++++++++++++
namespace {
std::ostream& operator<<(std::ostream& o, Block const& block) {
  char *const  code = strdup(block.code.c_str());
  
  // locate all "$$" and "$n" references
  char *apos = code;
  char *epos;

  while((epos = strchr(apos, '$')) != 0) {
    *epos = '\0';
    o << apos;

    switch(*++epos) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      o << "yystack.peek(yylen - " << strtoul(epos, &apos, 0) << ')';
      break;

    case '$':
      o << "yylval";
      apos = epos + 1;
      break;

    default:
      o << '$';
      apos = epos;
      break;
    }
  }
  o << apos << '\n';
  free(code);
  return  o;
}
}

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
JavaBackend::JavaBackend(Properties const& props)
  : name(props.getProperty("NAME")), generics(props.findProperty("gen") != 0) {}
JavaBackend::~JavaBackend() {}

//+ Output Generation ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void JavaBackend::accept(Grammar const& grm) {
  std::ofstream  java((name+".java").c_str());

  Klass       const& parser = grm.getParserClass();
  std::string const  sval   = grm.getSValueClass().toString(".");
  char const *const  sval0 =
    (sval=="char")? "'\0'" :
    (sval=="byte")||(sval=="short")||(sval=="int")||(sval=="long")? "0" :
    "null";    

  // package and imports
  if(parser.cntSpaces() > 0) {
    java << "package " << parser.getScope(".") << ";\n\n";
  }
  java << grm.getBlock(Blocks::HEADER_PREAMBLE) <<
    "public class " << parser.getName() << parser.getAdjunct() << "{\n"
    "  private static final boolean  yydebug = false;\n"
    "  private static final class YYStack {\n"
    "    private static final class Ele {\n"
    "      public final int  state;\n"
    "      public final " << sval << " sval;\n"
    "\n"
    "      public Ele(int state, " << sval << " sval) {\n"
    "        this.state = state;\n"
    "        this.sval  = sval;\n"
    "      }\n"
    "    }\n"
    "\n"
    "    private final java.util.ArrayList";
  if(generics)  java << "<Ele>";
  java << "  stack;\n"
    "\n"
    "    public YYStack() {\n"
    "      stack = new java.util.ArrayList";
  if(generics)  java << "<Ele>";
  java << "();\n"
    "    }\n"
    "    public void push(int state) {\n"
    "      stack.add(new Ele(state, " << sval0 << "));\n"
    "    }\n"
    "    public void push(int state, " << sval << " sval) {\n"
    "      stack.add(new Ele(state, sval));\n"
    "    }\n"
    "    public void pop(int cnt) {\n"
    "      int  size = stack.size();\n"
    "      stack.subList(size-cnt, size).clear();\n"
    "    }\n"
    "\n"
    "    public " << sval << " peek(int idx) {\n"
    "      return ((Ele)stack.get(stack.size()-1-idx)).sval;\n"
    "    }\n"
    "    public int top() {\n"
    "      return ((Ele)stack.get(stack.size()-1)).state;\n"
    "    }\n"
    "  }\n"
    "\n"
    "  private final static class YYSVal {\n"
    "    public " << sval << " val;\n" 
    "  }\n"
    "\n";
  java << grm.getBlock(Blocks::PARSER_INLINE);

  // Terminal definitions
  {
    TermPool const& terms = grm.getTerms();
    unsigned const  ex    = terms.lastExternal() + 1;
    
    // Constants for named terminals
    TermPool::Iterator  it = terms.iterator();
    while(it.hasNext()) {
      Terminal const  term = it.next();
      if(!term.isLiteral()) {
	java << "  public static final int  "  << terms.toString(term)
	     << " = " << term.external() << ";\n";
      }
      
    }
    java << "  private static final int  YYINTERN = " << ex << ";\n\n";
    
    // Calculate Mapping: external -> internal representation
    ushort *res = new ushort[ex];
    for(unsigned i = ex-1; i > 0; res[i--] = ex);
    res[0] = 0; // End-of-Input

    java << "  private static final String[]  yyterms = {\n\t\"EOF\",\n\t";
    for(TermPool::Iterator  it = terms.iterator(); it.hasNext();) {
      Terminal const  term = it.next();
      
      // Print name
      java << '\"' << terms.toString(term)
	   << (((term.internal()&7) == 0)? "\",\n\t": "\", ");
      
      // Mapping: external -> internal
      res[term.external()] = term.internal();
    }
    java << "\n  };\n";
    // Print mapping
    java << "  private static final short[]  yyintern = {\n\t";
    for(unsigned j = 0; j < ex;) {
      java.width(6);
      java << res[j++];
      java << (((j&7) == 0)? ",\n\t" : ", ");
    }
    java << "\n  };\n";
    
    delete [] res;
  }
  
  // Rule definitions
  {
    RulePool const& rules = grm.getRules();

    // Print string representation of rules for debugging
    java << "  private static final String[]  yyrules = {\n";
    for(RulePool::Iterator  it = rules.iterator(0); it.hasNext();) {
      Rule const  rule = it.next();
      java << "\t\"" << rules.toString(rule) << "\",\n";
    }
    java << "  };\n";

    // Print lengths of rules
    java << "private static final short[]  yylength = {\n\t";
    for(RulePool::Iterator  jt = rules.iterator(0); jt.hasNext();) {
      Rule const  rule = jt.next();
      java.width(6);
      java << rule.length();
      java << (((rule.number()&7) == 7)? ",\n\t" : ", ");
    }
    java << "\n  };\n";

    // Print left-hand sides turned positive
    java << "  private static final short[]  yylhs    = {\n\t";
    java << "   ~0, ";
    for(RulePool::Iterator  kt = rules.iterator(1); kt.hasNext();) {
      Rule const  rule = kt.next();
      java.width(6);
      java << ~rule.lhs();
      java << (((rule.number()&7) == 7)? ",\n\t" : ", ");
    }
    java << "\n  };\n\n";
  }

  // Parser Table definitions
  {
    ActionTable const& table = grm.getParserTable();

    unsigned    const  rows  = table.cntRows();
    unsigned    const  gcols = table.cntGotoCols();
    unsigned    const  acols = table.cntActionCols();
    
    // GOTO table
    java << "  private static final short[][]  yygoto = {\n";
    for(unsigned i = 0; i < rows; i++) {
      ActionTable::Row_const const  row = table[i];
    
      java << "\t{ ";
      for(unsigned j = 0; j < gcols; j++) {
	java.width(6);
	java << row[~j] << ", ";
      }
      java << " },\n";
    }
    java << "  };\n\n";

    // ACTION table
    java << "  private static final short[][]  yyaction = {\n";
    for(unsigned j = 0; j < rows; j++) {
      ActionTable::Row_const const  row = table[j];
      
      java << "\t{ ";
      for(unsigned k = 0; k < acols; k++) {
	java.width(6);
	java << row[k] << ", ";
      }
      java << " },\n";
    }
    java << "  };\n\n";
  }

  // Generate parse() method
  {
    ActionPool const& acts = grm.getActions();
    size_t     const  size = acts.size();

    // Generic Preamble
    java << 
      "public void parse() {\n"
      "  YYStack  yystack = new YYStack();\n"
      "  yystack.push(0);\n"
      "\n"
      "  // Fetch until error (throw) or accept (return)\n"
      "  while(true) {\n"
      "    // Current lookahead\n"
      "    YYSVal yysval = new YYSVal();\n"
      "    int    yytok  = nextToken(yysval);\n"
      "\n"
      "    if(yytok <  YYINTERN)  yytok = yyintern[yytok];\n"
      "    if(yytok >= YYINTERN)  error(\"Unknown Token\");\n"
      "    if(yydebug) {\n"
      "      System.err.println(\"Read \" + yyterms[yytok]);\n"
      "    }\n"
      "\n"
      "    // Reduce until shift\n"
      "    while(true) {\n"
      /*
	"      yystack.print();\n"
      */
      "      final short  yyact = yyaction[yystack.top()][yytok];\n"
      "      if(yyact == 0)  error(\"Syntax Error\");\n"
      "      if(yyact >  1) { // shift\n"
      "        if(yydebug) {\n"
      "          System.err.println(\"Push \" + yyterms[yytok]);\n"
      "        }\n"
      "        yystack.push(yyact, yysval.val);\n"
      "        break;\n"
      "      }\n"
      "      else {           // reduce (includes accept)\n"
      "        " << sval << " yylval = " << sval0 << ";\n"
      "        final int  yyrno = (yyact < 0)? -yyact : 0;\n"
      "        final int  yylen = yylength[yyrno];\n"
      "\n"
      "        if(yydebug) {\n"
      "          System.err.println(\"Reduce by \" + yyrules[yyrno]);\n"
      "        }\n"
      "        switch(yyrno) { // do semantic actions\n"
      "        case 0:         // accept\n"
      "          return;\n";

    // Specific Reduction Actions
    for(size_t i = 0; i < size; i++) {
      Block const& act = acts.get(i);
      if(!act.code.empty()) {
	java << "case " << i << ": {\n";
	java << act
	     << "break;\n}\n";
      }
    }
    
    // Generic Postamble
    java <<
      "        }\n"
      "        \n"
      "        yystack.pop(yylen);\n"
      "        yystack.push(yygoto[yystack.top()][yylhs[yyrno]], yylval);\n"
      "      }\n"
      "    }\n"
      "  }\n"
      "}\n";
  }
  
  // close class
  java << "}\n";
}
