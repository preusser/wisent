#line 35 "WP.ypp"

#include "WParser.hpp"

#include "WBuilder.hpp"
#include "../../Terminal.hpp"

#include <stdlib.h>

using namespace wisent;
using namespace wisent::frontend::wisent;

unsigned WParser::nextToken(YYSVal& sval) {
  int  tok = scanner.nextToken();
  if(tok < 0)  error(scanner.getText());

  switch(tok) {
  case BLOCK:
    sval.line = scanner.getMark();
  case LIT:
  case IDENT:
  case KLASS:
    sval.code = scanner.getText();
  }
  return  tok;
}

void WParser::error(std::string const& msg) const {
  std::cerr << msg << " in line " << scanner.lineno() << std::endl;
  exit(1);
}


#line 34 "WP.cpp"
#include <vector>
class wisent::frontend::wisent::WParser::YYStack {
  class Ele {
  public:
    unsigned short  state;
    YYSVal          sval;

  public:
    Ele(unsigned short _state, YYSVal const& _sval)
     : state(_state), sval(_sval) {}
    Ele(unsigned short _state)
     : state(_state) {}
  };
  typedef std::vector<Ele>  Stack;
  Stack  stack;

public:
  YYStack() {}
  ~YYStack() {}

public:
  void push(unsigned short state) {
    stack.push_back(Ele(state));
  }
  void push(unsigned short state, YYSVal const& sval) {
    stack.push_back(Ele(state, sval));
  }
  void pop(unsigned cnt) {
    Stack::iterator  it = stack.end();
    stack.erase(it-cnt, it);
  }

  YYSVal& operator[](unsigned idx) { return  (stack.rbegin()+idx)->sval; }
  unsigned short operator*() const { return  stack.back().state; }
};

#ifdef TRACE
char const *const  wisent::frontend::wisent::WParser::yyterms[] = { "EOF", 
"BREAK", "HEADER", "SVAL", "CLASS", "IMPL", "TOKEN", "LEFT", "RIGHT",
"NONASSOC", "START", "LIT", "IDENT", "KLASS", "BLOCK", "'*'", "'<'",
"'>'", "','", "':'", "'?'", "'|'", "';'", };
#endif

unsigned short const  wisent::frontend::wisent::WParser::yyintern[] = {
     0,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,     15,    270,     18,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,     19,     22,     16,    270,     17,     20,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,     21,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
   270,    270,    270,    270,    270,    270,    270,    270,
     1,      2,      3,      4,      5,      6,      7,      8,
     9,     10,     11,     12,     13,     14, };

#ifdef TRACE
char const *const  wisent::frontend::wisent::WParser::yyrules[] = {
"   0: [ 0] $        -> gram",
"   1: [ 1] gram     -> opt BREAK decl BREAK spec",
"   2: [ 0] opt      -> header klassd svald impl",
"   3: [ 5] header   -> HEADER BLOCK",
"   4: [ 5] klassd   -> CLASS klass adjunct BLOCK",
"   5: [ 2] svald    -> SVAL sval",
"   6: [ 0] sval     -> klass",
"   7: [ 0] sval     -> sval '*'",
"   8: [ 0] sval     -> klass '<' svall '>'",
"   9: [ 0] svall    -> svall ',' sval",
"  10: [ 0] svall    -> sval",
"  11: [ 5] klass    -> KLASS",
"  12: [ 5] klass    -> IDENT",
"  13: [ 5] impl     -> IMPL BLOCK",
"  14: [ 0] adjunct  -> adjunct klass",
"  15: [ 0] adjunct  -> adjunct ':'",
"  16: [ 0] adjunct  -> adjunct '<'",
"  17: [ 0] adjunct  -> adjunct '>'",
"  18: [ 0] adjunct  -> adjunct '?'",
"  19: [ 0] adjunct  -> adjunct ','",
"  20: [ 0] adjunct  ->",
"  21: [ 5] decl     -> tokdecl START IDENT",
"  22: [ 0] tokdecl  -> tokdecl tokdef",
"  23: [ 0] tokdecl  ->",
"  24: [ 0] tokdef   -> assoc toklist",
"  25: [ 3] assoc    -> TOKEN",
"  26: [ 3] assoc    -> LEFT",
"  27: [ 3] assoc    -> RIGHT",
"  28: [ 3] assoc    -> NONASSOC",
"  29: [ 0] toklist  -> toklist token",
"  30: [ 0] toklist  -> token",
"  31: [ 5] token    -> LIT",
"  32: [ 5] token    -> IDENT",
"  33: [ 0] spec     -> spec rule",
"  34: [ 0] spec     ->",
"  35: [ 0] rule     -> lhs ':' rhs altlist term",
"  36: [ 5] lhs      -> IDENT",
"  37: [ 5] rhs      -> rhs LIT",
"  38: [ 5] rhs      -> rhs IDENT",
"  39: [ 5] rhs      ->",
"  40: [ 0] altlist  -> altlist alt rhs",
"  41: [ 0] altlist  ->",
"  42: [ 0] alt      -> term '|'",
"  43: [ 0] alt      -> '|'",
"  44: [ 0] term     -> ';'",
"  45: [ 5] term     -> BLOCK",
"  46: [ 0] term     -> BLOCK ';'",
};
#endif
unsigned short const wisent::frontend::wisent::WParser::yylength[] = {
     1,      5,      4,      2,      4,      2,      1,      2,
     4,      3,      1,      1,      1,      2,      2,      2,
     2,      2,      2,      2,      0,      3,      2,      0,
     2,      1,      1,      1,      1,      2,      1,      1,
     1,      2,      0,      5,      1,      2,      2,      0,
     3,      0,      2,      1,      1,      1,      2, };
unsigned short const wisent::frontend::wisent::WParser::yylhs   [] = {
   (unsigned short)~0u,      0,      1,      4,      5,      6,     10,     10,
    10,     11,     11,      8,      8,      7,      9,      9,
     9,      9,      9,      9,      9,      2,     12,     12,
    13,     14,     14,     14,     14,     15,     15,     16,
    16,      3,      3,     17,     18,     19,     19,     19,
    20,     20,     22,     22,     21,     21,     21, };

unsigned short const  wisent::frontend::wisent::WParser::yygoto  [][23] = {
{      3,      2,      0,      0,      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,     40,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      8,      0,      0,      0,      0,      0,      0,      0,      0,      0,      7,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     28,     27,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,     10,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     12,     11,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     15,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     16,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     20,     19,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     26,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     36,     35,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     39,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,     53,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,     42,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,     45,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,     46,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,     64,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,     56,      0,     55,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,     56,      0,     59,     58,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,     56,      0,     63,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
};

signed short const  wisent::frontend::wisent::WParser::yyaction[][23] = {
{      0,      0,      4,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,     41,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      6,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      5,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,     -3,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -23,    -23,    -23,    -23,    -23,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,     29,     30,     31,     32,     33,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      9,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{    -34,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -34,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{     -1,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     14,      0,      0,      0,  },
{    -33,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -33,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -36,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -39,    -39,      0,    -39,      0,      0,      0,      0,      0,      0,    -39,    -39,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     17,     18,      0,    -41,      0,      0,      0,      0,      0,      0,    -41,    -41,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     21,      0,      0,      0,      0,      0,      0,     22,     23,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -37,    -37,      0,    -37,      0,      0,      0,      0,      0,      0,    -37,    -37,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -38,    -38,      0,    -38,      0,      0,      0,      0,      0,      0,    -38,    -38,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -39,    -39,      0,    -39,      0,      0,      0,      0,      0,      0,    -39,    -39,  },
{    -35,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -35,      0,      0,      0,      0,      0,      0,      0,      0,     25,      0,  },
{    -45,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -45,      0,      0,      0,      0,      0,      0,      0,      0,    -45,     24,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -43,    -43,      0,    -43,      0,      0,      0,      0,      0,      0,    -43,    -43,  },
{    -44,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -44,      0,      0,      0,      0,      0,      0,      0,      0,    -44,      0,  },
{    -46,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -46,      0,      0,      0,      0,      0,      0,      0,      0,    -46,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -42,    -42,      0,    -42,      0,      0,      0,      0,      0,      0,    -42,    -42,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     17,     18,      0,    -40,      0,      0,      0,      0,      0,      0,    -40,    -40,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     37,     38,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -22,    -22,    -22,    -22,    -22,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -25,    -25,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -26,    -26,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -27,    -27,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -28,    -28,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     34,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,    -21,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -30,    -30,    -30,    -30,    -30,    -30,    -30,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -24,    -24,    -24,    -24,    -24,     37,     38,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -31,    -31,    -31,    -31,    -31,    -31,    -31,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -32,    -32,    -32,    -32,    -32,    -32,    -32,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,    -29,    -29,    -29,    -29,    -29,    -29,    -29,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,     54,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     43,     44,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -20,    -20,    -20,      0,    -20,    -20,    -20,    -20,    -20,      0,      0,  },
{      0,      0,      0,      0,      0,    -12,      0,      0,      0,      0,      0,      0,    -12,    -12,    -12,    -12,    -12,    -12,    -12,    -12,    -12,      0,      0,  },
{      0,      0,      0,      0,      0,    -11,      0,      0,      0,      0,      0,      0,    -11,    -11,    -11,    -11,    -11,    -11,    -11,    -11,    -11,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     43,     44,     47,      0,     48,     49,     50,     51,     52,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -14,    -14,    -14,      0,    -14,    -14,    -14,    -14,    -14,      0,      0,  },
{      0,      0,      0,     -4,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -16,    -16,    -16,      0,    -16,    -16,    -16,    -16,    -16,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -17,    -17,    -17,      0,    -17,    -17,    -17,    -17,    -17,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -19,    -19,    -19,      0,    -19,    -19,    -19,    -19,    -19,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -15,    -15,    -15,      0,    -15,    -15,    -15,    -15,    -15,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,    -18,    -18,    -18,      0,    -18,    -18,    -18,    -18,    -18,      0,      0,  },
{      0,      0,      0,      0,      0,     65,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     43,     44,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,     -5,      0,      0,      0,      0,      0,      0,      0,      0,      0,     60,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,     -6,      0,      0,      0,      0,      0,      0,      0,      0,      0,     -6,     57,     -6,     -6,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     43,     44,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     61,     62,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     60,      0,    -10,    -10,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,     -7,      0,      0,      0,      0,      0,      0,      0,      0,      0,     -7,      0,     -7,     -7,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,     -8,      0,      0,      0,      0,      0,      0,      0,      0,      0,     -8,      0,     -8,     -8,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     43,     44,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     60,      0,     -9,     -9,      0,      0,      0,      0,  },
{      0,     -2,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,     66,      0,      0,      0,      0,      0,      0,      0,      0,  },
{      0,    -13,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,  },
};

void wisent::frontend::wisent::WParser::parse() {
  YYStack  yystack;
  yystack.push(0);

  // Fetch until error (throw) or accept (return)
  while(true) {
    // Current lookahead
    YYSVal          yysval;
    unsigned short  yytok = nextToken(yysval);

    if(yytok <  YYINTERN)  yytok = yyintern[yytok];
    if(yytok >= YYINTERN)  error("Unknown Token");
#ifdef TRACE
    std::cerr << "Read " << yyterms[yytok] << std::endl;
#endif

    // Reduce until shift
    while(true) {
      signed short const  yyact = yyaction[*yystack][yytok];
      if(yyact == 0)  error("Syntax Error");
      if(yyact >  1) { // shift
#ifdef TRACE
        std::cerr << "Push " << yyterms[yytok] << std::endl;
#endif
        yystack.push(yyact, yysval);
        break;
      }
      else {           // reduce (includes accept)
        YYSVal                yylval;
        unsigned short const  yyrno = (yyact < 0)? -yyact : 0;
        unsigned short const  yylen = yylength[yyrno];
        
#ifdef TRACE
        std::cerr << "Reduce by " << yyrules[yyrno] << std::endl;
#endif
        switch(yyrno) { // do semantic actions
        case 0:         // accept
          return;
case 3: {
#line 81 "WP.ypp"
 bld.setHeaderPreamble(yystack[yylen - 2]); 
#line 361 "WP.cpp"
break;
}
case 4: {
#line 82 "WP.ypp"
 
	    bld.setParserClass (Klass(yystack[yylen - 2].code, yystack[yylen - 3].code));
            bld.setParserInline(yystack[yylen - 4]);
          
#line 370 "WP.cpp"
break;
}
case 5: {
#line 86 "WP.ypp"
 bld.setSValueClass(Klass(yystack[yylen - 2].code)); 
#line 376 "WP.cpp"
break;
}
case 6: {
#line 87 "WP.ypp"
 yylval.code = yystack[yylen - 1].code; 
#line 382 "WP.cpp"
break;
}
case 7: {
#line 88 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + '*'; 
#line 388 "WP.cpp"
break;
}
case 8: {
#line 89 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + "< " + yystack[yylen - 3].code + " >"; 
#line 394 "WP.cpp"
break;
}
case 9: {
#line 90 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + ", " + yystack[yylen - 3].code; 
#line 400 "WP.cpp"
break;
}
case 10: {
#line 91 "WP.ypp"
 yylval.code = yystack[yylen - 1].code; 
#line 406 "WP.cpp"
break;
}
case 11: {
#line 93 "WP.ypp"
 yylval.code = yystack[yylen - 1].code; 
#line 412 "WP.cpp"
break;
}
case 12: {
#line 94 "WP.ypp"
 yylval.code = yystack[yylen - 1].code; 
#line 418 "WP.cpp"
break;
}
case 13: {
#line 95 "WP.ypp"
 bld.setParserImpl(yystack[yylen - 2]); 
#line 424 "WP.cpp"
break;
}
case 14: {
#line 96 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + ' ' + yystack[yylen - 2].code; 
#line 430 "WP.cpp"
break;
}
case 15: {
#line 97 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + " :"; 
#line 436 "WP.cpp"
break;
}
case 16: {
#line 98 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + " <"; 
#line 442 "WP.cpp"
break;
}
case 17: {
#line 99 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + " >"; 
#line 448 "WP.cpp"
break;
}
case 18: {
#line 100 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + '?'; 
#line 454 "WP.cpp"
break;
}
case 19: {
#line 101 "WP.ypp"
 yylval.code = yystack[yylen - 1].code + ','; 
#line 460 "WP.cpp"
break;
}
case 20: {
#line 102 "WP.ypp"
 yylval.code = ""; 
#line 466 "WP.cpp"
break;
}
case 21: {
#line 105 "WP.ypp"
 bld.startSymbol(yystack[yylen - 3].code); 
#line 472 "WP.cpp"
break;
}
case 25: {
#line 112 "WP.ypp"
 bld.setAssoc(Terminal::NONE); 
#line 478 "WP.cpp"
break;
}
case 26: {
#line 113 "WP.ypp"
 bld.setAssoc(Terminal::LEFT); 
#line 484 "WP.cpp"
break;
}
case 27: {
#line 114 "WP.ypp"
 bld.setAssoc(Terminal::RIGHT); 
#line 490 "WP.cpp"
break;
}
case 28: {
#line 115 "WP.ypp"
 bld.setAssoc(Terminal::NONE); 
#line 496 "WP.cpp"
break;
}
case 31: {
#line 120 "WP.ypp"
 bld.defineTerminal(yystack[yylen - 1].code[0]); 
#line 502 "WP.cpp"
break;
}
case 32: {
#line 121 "WP.ypp"
 bld.defineTerminal(yystack[yylen - 1].code); 
#line 508 "WP.cpp"
break;
}
case 36: {
#line 129 "WP.ypp"
 bld.startRule(yystack[yylen - 1].code); 
#line 514 "WP.cpp"
break;
}
case 37: {
#line 131 "WP.ypp"
 bld.appendRule(yystack[yylen - 2].code[0]); 
#line 520 "WP.cpp"
break;
}
case 38: {
#line 132 "WP.ypp"
 bld.appendRule(yystack[yylen - 2].code); 
#line 526 "WP.cpp"
break;
}
case 42: {
#line 138 "WP.ypp"
			bld.startAlternative(); 
#line 532 "WP.cpp"
break;
}
case 43: {
#line 139 "WP.ypp"
 bld.actionRule(); bld.startAlternative(); 
#line 538 "WP.cpp"
break;
}
case 44: {
#line 140 "WP.ypp"
 bld.actionRule();   
#line 544 "WP.cpp"
break;
}
case 45: {
#line 141 "WP.ypp"
 bld.actionRule(yystack[yylen - 1]); 
#line 550 "WP.cpp"
break;
}
case 46: {
#line 142 "WP.ypp"
 bld.actionRule(yystack[yylen - 1]); 
#line 556 "WP.cpp"
break;
}
        }
        
        yystack.pop(yylen);
        yystack.push(yygoto[*yystack][yylhs[yyrno]], yylval);
      }
    }
  }
}
