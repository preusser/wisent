#ifndef WISENT_FRONTEND_WISENT_WPARSER_HPP_INCLUDED
#define WISENT_FRONTEND_WISENT_WPARSER_HPP_INCLUDED
#line 1 "WP.ypp"

namespace wisent {
  namespace frontend {
    namespace wisent {
      class WBuilder;
    }
  }
}
#include "../../Block.hpp"
#include "WScanner.hpp"
#include <iostream>

#line 16 "WP.hpp"
#include <string>
namespace wisent {
namespace frontend {
namespace wisent {
class WParser {
  typedef Block YYSVal;
  class YYStack;
#line 14 "WP.ypp"

private:
  WBuilder& bld;
  WScanner  scanner;

private:
  WParser(WBuilder& b, std::istream& in) : bld(b), scanner(in) {}
  ~WParser() {}

public:
  static void parse(WBuilder& bld, std::istream& in) {
    WParser(bld, in).parse();
  }

private:
  unsigned nextToken(YYSVal& sval);
  void error(std::string const& msg) const;

#line 43 "WP.hpp"
private:
  void parse();
public:
enum {
  BREAK = 256,
  HEADER = 257,
  SVAL = 258,
  CLASS = 259,
  IMPL = 260,
  TOKEN = 261,
  LEFT = 262,
  RIGHT = 263,
  NONASSOC = 264,
  START = 265,
  LIT = 266,
  IDENT = 267,
  KLASS = 268,
  BLOCK = 269,
};
private:
enum { YYINTERN = 270 };
static unsigned short const  yyintern[];
static char const    *const  yyterms[];

private:
static unsigned short const  yylength[];
static unsigned short const  yylhs   [];
static char const    *const  yyrules [];

private:
static unsigned short const  yygoto  [][23];
static signed   short const  yyaction[][23];
};
}}}
#endif
