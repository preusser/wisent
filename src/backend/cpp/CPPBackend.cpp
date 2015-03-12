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

#include "CPPBackend.hpp"

#include "MarkStream.hpp"

#include "../../Properties.hpp"
#include "../../Grammar.hpp"

#include <string.h>
#include <stdlib.h>
#include <fstream>

using namespace wisent;
using namespace wisent::backend::cpp;

//+ Output of Code Blocks ++++++++++++++++++++++++++++++++++++++++++++++++++++
MarkStream& operator<<(MarkStream& o, Block const& block) {
  o.mark(block.file, block.line);

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
      o << "yystack[yylen - " << strtoul(epos, &apos, 0) << ']';
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
  o.mark();
  return  o;
}

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
namespace {
  char const *const  OFFS[] = { "no", "off", "none", "false", "0" };
  bool is_off(Properties const& props, char const *const  name) {
    char const *const  val = props.findProperty(name);
    if(val != 0) {
      for(unsigned  i = 0; i < sizeof(OFFS)/sizeof(*OFFS); i++) {
	if(strcasecmp(val, OFFS[i]) == 0)  return  true;
      }
    }
    return  false;
  }
}

CPPBackend::CPPBackend(Properties const& props)
  : name(props.getProperty("NAME")), lineinfo(!is_off(props, "lineinfo")) {}
CPPBackend::~CPPBackend() {}

//+ Output Generation ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void CPPBackend::accept(Grammar const& grm) {
  buildHeader(grm);
  buildImpl(grm);
}

void CPPBackend::buildHeader(Grammar const& grm) const {
  MarkStream  hpp(name + ".hpp", lineinfo);

  Klass    const& parser = grm.getParserClass();
  unsigned const  depth  = parser.cntSpaces();

  // generate header symbol and open class
  {
    std::string  hdrsym;
    std::string  spaces;

    for(unsigned  i = 0; i < depth; i++) {
      std::string const& space = parser.getSpace(i);
      unsigned    const  len   = space.size();
      for(unsigned  j = 0; j < len; hdrsym += toupper(space[j++]));
      hdrsym += '_';
      spaces += "namespace " + space + " {\n";
    }
    
    std::string const& name = parser.getName();
    unsigned    const  len  = name.size();
    for(unsigned  j = 0; j < len; hdrsym += toupper(name[j++]));
    hdrsym += "_HPP_INCLUDED\n";

    hpp << "#ifndef " << hdrsym
	<< "#define " << hdrsym;
    hpp << grm.getBlock(Blocks::HEADER_PREAMBLE)
	<< "#include <string>\n"
	<< spaces << "class " << name << parser.getAdjunct() << " {\n";
  }

  // common members and typedefs
  {
    Klass    const& sval = grm.getSValueClass();
    unsigned const  dpth = sval.cntSpaces();

    hpp << "  typedef ";
    for(unsigned  i = 0; i < dpth; hpp << sval.getSpace(i++) << "::");
    hpp << sval.getName() << " YYSVal;\n"
           "  class YYStack;\n";
    hpp << grm.getBlock(Blocks::PARSER_INLINE)
	<< "private:\n  void parse();\n";
  }

  // Terminal declarations
  {
    TermPool const& terms = grm.getTerms();

    // Constants for named terminals
    hpp << "public:\nenum {\n";
    TermPool::Iterator  it = terms.iterator();
    while(it.hasNext()) {
      Terminal const  term = it.next();
      if(!term.isLiteral()) {
	hpp << "  "  << terms.toString(term)
	    << " = " << term.external() << ",\n";
      }

    }

    hpp << "};\nprivate:\nenum { YYINTERN = "
	<< (terms.lastExternal()+1) << " };\n"
           "static unsigned short const  yyintern[];\n"
           "static char const    *const  yyterms[];\n\n";
  }

  // Rule declarations
  hpp << "private:\n"
         "static unsigned short const  yylength[];\n"
         "static unsigned short const  yylhs   [];\n"
         "static char const    *const  yyrules [];\n\n";

  // Parser Table declarations
  {
    ActionTable const& table = grm.getParserTable();

    hpp << "private:\n"
           "static unsigned short const  yygoto  []["
	<< table.cntGotoCols() << "];\n"
           "static signed   short const  yyaction[]["
	<< table.cntActionCols() << "];\n";
  }

  // close class
  hpp << "};\n" << std::string(depth, '}') << "\n"
         "#endif\n";
}

void CPPBackend::buildImpl(Grammar const& grm) const {
  MarkStream  cpp(name + ".cpp", lineinfo);

  Klass       const& parser = grm.getParserClass();
  std::string const  scope  = parser.toString("::") + "::";

  // output user-specific and own stack implementation
  cpp << grm.getBlock(Blocks::PARSER_IMPLEMENTATION) <<
    "#include <vector>\n"
    "class " << scope << "YYStack {\n"
    "  class Ele {\n"
    "  public:\n"
    "    unsigned short  state;\n"
    "    YYSVal          sval;\n"
    "\n"
    "  public:\n"
    "    Ele(unsigned short _state, YYSVal const& _sval)\n"
    "     : state(_state), sval(_sval) {}\n"
    "    Ele(unsigned short _state)\n"
    "     : state(_state) {}\n"
    "  };\n"
    "  typedef std::vector<Ele>  Stack;\n"
    "  Stack  stack;\n"
    "\n"
    "public:\n"
    "  YYStack() {}\n"
    "  ~YYStack() {}\n"
    "\n"
    "public:\n"
    "  void push(unsigned short state) {\n"
    "    stack.push_back(Ele(state));\n"
    "  }\n"
    "  void push(unsigned short state, YYSVal const& sval) {\n"
    "    stack.push_back(Ele(state, sval));\n"
    "  }\n"
    "  void pop(unsigned cnt) {\n"
    "    Stack::iterator  it = stack.end();\n"
    "    stack.erase(it-cnt, it);\n"
    "  }\n"
    "\n"
    "  YYSVal& operator[](unsigned idx) { return  (stack.rbegin()+idx)->sval; }\n"
    "  unsigned short operator*() const { return  stack.back().state; }\n"
/*
    "\n"
    "void print() const {\n"
    "  for(Stack::const_iterator  it = stack.begin(); it != stack.end(); it++) {\n"
    "    std::cout << '(' << it->state << ':' << it->sval << \") \";\n"
    "  }\n"
    "  std::cout << std::endl;\n"
    "}\n"
*/
    "};\n\n";

  // terminal data
  {
    TermPool const& terms = grm.getTerms();
    unsigned const  ex    = terms.lastExternal() + 1;

    // Calculate Mapping: external -> internal representation
    ushort *res = new ushort[ex];
    for(unsigned i = ex-1; i > 0; res[i--] = ex);
    res[0] = 0; // End-of-Input

    cpp << "char const *const  " << scope << "yyterms[] = { \"EOF\", \n";
    for(TermPool::Iterator  it = terms.iterator(); it.hasNext();) {
      Terminal const  term = it.next();

      // Print name
      cpp << '"' << terms.toString(term) << "\","
	  << (((term.internal()&7) == 0)? '\n' : ' ');

      // Mapping: external -> internal
      res[term.external()] = term.internal();
    }
    cpp << "};\n";

    // Print mapping
    cpp << "unsigned short const  " << scope << "yyintern[] = {\n";
    for(unsigned j = 0; j < ex;) {
      cpp.width(6);
      cpp << res[j++];
      cpp << (((j&7) == 0)? ",\n" : ", ");
    }
    cpp << "};\n" << std::endl;

    delete [] res;
  }

  // rule data
  {
    RulePool const& rules = grm.getRules();

    // Print string representation of rules for debugging
    cpp << "#ifdef TRACE\n"
           "char const *const  " << scope << "yyrules[] = {\n";
    for(RulePool::Iterator  it = rules.iterator(0); it.hasNext();) {
      Rule const  rule = it.next();
      cpp << '"' << rules.toString(rule) << "\",\n";
    }
    cpp << "};\n"
           "#endif\n";

    // Print lengths of rules
    cpp << "unsigned short const " << scope << "yylength[] = {\n";
    for(RulePool::Iterator  jt = rules.iterator(0); jt.hasNext();) {
      Rule const  rule = jt.next();
      cpp.width(6);
      cpp << rule.length();
      cpp << (((rule.number()&7) == 7)? ",\n" : ", ");
    }
    cpp << "};\n";

    // Print left-hand sides turned positive
    cpp << "unsigned short const " << scope << "yylhs   [] = {\n";
    cpp << "   (unsigned short)~0u, ";
    for(RulePool::Iterator  kt = rules.iterator(1); kt.hasNext();) {
      Rule const  rule = kt.next();
      cpp.width(6);
      cpp << ~rule.lhs();
      if((rule.number()&7) == 7)  cpp << ",\n";
      else  cpp << ", ";
    }
    cpp << "};\n\n";
  }

  // parser tables
  {
    ActionTable const& table = grm.getParserTable();
    unsigned    const  rows  = table.cntRows();
    unsigned    const  gcols = table.cntGotoCols();
    unsigned    const  acols = table.cntActionCols();
    
    // GOTO table
    cpp << "unsigned short const  " << scope << "yygoto  [][" << gcols << "] = {\n";
    for(unsigned i = 0; i < rows; i++) {
      ActionTable::Row_const const  row = table[i];
    
      cpp << "{ ";
      for(unsigned j = 0; j < gcols; j++) {
	cpp.width(6);
	cpp << row[~j] << ", ";
      }
      cpp << " },\n";
    }
    cpp << "};\n\n";

    // ACTION table
    cpp << "signed short const  " << scope << "yyaction[][" << acols << "] = {\n";
    for(unsigned j = 0; j < rows; j++) {
      ActionTable::Row_const const  row = table[j];
      
      cpp << "{ ";
      for(unsigned k = 0; k < acols; k++) {
	cpp.width(6);
	cpp << row[k] << ", ";
      }
      cpp << " },\n";
    }
    cpp << "};\n\n";

    // Generate parse() method
    ActionPool const& acts = grm.getActions();
    size_t     const  size = acts.size();

    // Generic Preamble
    cpp << 
      "void " << scope << "parse() {\n"
      "  YYStack  yystack;\n"
      "  yystack.push(0);\n"
      "\n"
      "  // Fetch until error (throw) or accept (return)\n"
      "  while(true) {\n"
      "    // Current lookahead\n"
      "    YYSVal          yysval;\n"
      "    unsigned short  yytok = nextToken(yysval);\n"
      "\n"
      "    if(yytok <  YYINTERN)  yytok = yyintern[yytok];\n"
      "    if(yytok >= YYINTERN)  error(\"Unknown Token\");\n"
      "#ifdef TRACE\n"
      "    std::cerr << \"Read \" << yyterms[yytok] << std::endl;\n"
      "#endif\n"
      "\n"
      "    // Reduce until shift\n"
      "    while(true) {\n"
      /*
	"      yystack.print();\n"
      */
      "      signed short const  yyact = yyaction[*yystack][yytok];\n"
      "      if(yyact == 0) {\n"
      "        std::string                yymsg(\"Expecting (\");\n"
      "        signed short const *const  yyrow = yyaction[*yystack];\n"
      "        for(unsigned  i = 0; i < " << acols << "; i++) {\n"
      "          if(yyrow[i])  yymsg.append(yyterms[i]) += '|';\n"
      "        }\n"
      "        *yymsg.rbegin() = ')';\n"
      "        error(yymsg.append(\" instead of \").append(yyterms[yytok]));\n"
      "        return;\n"
      "      }\n"
      "      if(yyact >  1) { // shift\n"
      "#ifdef TRACE\n"
      "        std::cerr << \"Push \" << yyterms[yytok] << std::endl;\n"
      "#endif\n"
      "        yystack.push(yyact, yysval);\n"
      "        break;\n"
      "      }\n"
      "      else {           // reduce (includes accept)\n"
      "        YYSVal                yylval;\n"
      "        unsigned short const  yyrno = (yyact < 0)? -yyact : 0;\n"
      "        unsigned short const  yylen = yylength[yyrno];\n"
      "        \n"
      "#ifdef TRACE\n"
      "        std::cerr << \"Reduce by \" << yyrules[yyrno] << std::endl;\n"
      "#endif\n"
      "        switch(yyrno) { // do semantic actions\n"
      "        case 0:         // accept\n"
      "          return;\n";

    // Specific Reduction Actions
    for(size_t i = 0; i < size; i++) {
      Block const& act = acts.get(i);
      if(!act.code.empty()) {
	cpp << "case " << i << ": {\n";
	cpp << act
	    << "break;\n}\n";
      }
    }
    
    // Generic Postamble
    cpp <<
      "        }\n"
      "        \n"
      "        yystack.pop(yylen);\n"
      "        yystack.push(yygoto[*yystack][yylhs[yyrno]], yylval);\n"
      "      }\n"
      "    }\n"
      "  }\n"
      "}\n";
  }
}
