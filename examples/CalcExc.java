
import  java.io.InputStream;
import  java.io.InputStreamReader;
import  java.io.StreamTokenizer;
import  java.io.IOException;

import  java.text.ParseException;

public class CalcExc extends Object{
  private static final boolean  yydebug = false;
  private static final class YYStack {
    private static final class Ele {
      public final int  state;
      public final int sval;

      public Ele(int state, int sval) {
        this.state = state;
        this.sval  = sval;
      }
    }

    private final java.util.List  stack;

    public YYStack() {
      stack = new java.util.ArrayList();
    }
    public void push(int state) {
      stack.add(new Ele(state, 0));
    }
    public void push(int state, int sval) {
      stack.add(new Ele(state, sval));
    }
    public void pop(int cnt) {
      int  size = stack.size();
      stack.subList(size-cnt, size).clear();
    }

    public int peek(int idx) {
      return ((Ele)stack.get(stack.size()-1-idx)).sval;
    }
    public int top() {
      return ((Ele)stack.get(stack.size()-1)).state;
    }
  }

  private final static class YYSVal {
    public int val;
  }


  StreamTokenizer  tok;

 /** Inner RuntimeException to wrap checked Exceptions. */
  private static class WrappingRuntimeException extends RuntimeException {
    public WrappingRuntimeException(Throwable cause) { super(cause); }
  }


  private CalcExc(InputStream  in) {
    tok = new StreamTokenizer(new InputStreamReader(in));
    tok.eolIsSignificant(true);
    tok.ordinaryChar('-');
  }  

  public static void parse(InputStream  in) throws IOException, ParseException {
    try {
      new CalcExc(in).parse();
    }
    // Unwrap the wrapped Exceptions of the expected and only possible types.
    catch(WrappingRuntimeException e) {
      Throwable  t = e.getCause();
      if(t instanceof IOException   )  throw (IOException)   t;
      if(t instanceof ParseException)  throw (ParseException)t;
      throw  e;
    }
  }
  public static void main(String[] args) {
    try {
      parse(System.in);
    }
    catch(Exception e) {
      System.out.println(e);
    }
  }

  private int nextToken(YYSVal val) {
   try {
    int  token = tok.nextToken();

    switch(token) {
    case StreamTokenizer.TT_NUMBER:
      val.val = Math.round((float)tok.nval);
      return  NUM;
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
      return  token;
    case StreamTokenizer.TT_EOL:
      return  RET;
    case StreamTokenizer.TT_EOF:
      return  0;
    case StreamTokenizer.TT_WORD:
      error("Unknown Token: " + tok.sval);
    }
    return  0;
   }
   // Wrap IOException
   catch(IOException e) {
     throw  new WrappingRuntimeException(e);
   }
  }
  void error(String msg) {
    // Wrap the ParseException to be thrown
    throw  new WrappingRuntimeException(new ParseException(msg, -1));
  }

  public static final int  NUM = 256;
  public static final int  RET = 257;
  private static final int  YYINTERN = 258;

  private static final String[]  yyterms = {
	"EOF",
	"NUM", "RET", "'+'", "'-'", "'*'", "'/'", "'('", "')'",
	
  };
  private static final short[]  yyintern = {
	     0,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	     7,      8,      5,      3,    258,      4,    258,      6,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	   258,    258,    258,    258,    258,    258,    258,    258,
	     1,      2, 
  };
  private static final String[]  yyrules = {
	"   0: [ 0] $        -> eqs",
	"   1: [ 1] eqs      -> eqs eq RET",
	"   2: [ 1] eqs      ->",
	"   3: [ 0] eq       -> expr",
	"   4: [ 1] expr     -> NUM",
	"   5: [ 0] expr     -> '(' expr ')'",
	"   6: [ 2] expr     -> expr '+' expr",
	"   7: [ 2] expr     -> expr '-' expr",
	"   8: [ 3] expr     -> expr '*' expr",
	"   9: [ 3] expr     -> expr '/' expr",
  };
private static final short[]  yylength = {
	     1,      3,      0,      1,      1,      3,      3,      3,
	     3,      3, 
  };
  private static final short[]  yylhs    = {
	   ~0,      0,      0,      1,      2,      2,      2,      2,
	     2,      2, 
  };

  private static final short[][]  yygoto = {
	{      1,      0,      0,  },
	{      0,      3,      2,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
	{      0,      0,      6,  },
	{      0,      0,      0,  },
	{      0,      0,     15,  },
	{      0,      0,     14,  },
	{      0,      0,     13,  },
	{      0,      0,     12,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
	{      0,      0,      0,  },
  };

  private static final short[][]  yyaction = {
	{     -2,     -2,      0,      0,      0,      0,      0,     -2,      0,  },
	{      1,      4,      0,      0,      0,      0,      0,      5,      0,  },
	{      0,      0,     -3,      7,      8,      9,     10,      0,      0,  },
	{      0,      0,     16,      0,      0,      0,      0,      0,      0,  },
	{      0,      0,     -4,     -4,     -4,     -4,     -4,      0,     -4,  },
	{      0,      4,      0,      0,      0,      0,      0,      5,      0,  },
	{      0,      0,      0,      7,      8,      9,     10,      0,     11,  },
	{      0,      4,      0,      0,      0,      0,      0,      5,      0,  },
	{      0,      4,      0,      0,      0,      0,      0,      5,      0,  },
	{      0,      4,      0,      0,      0,      0,      0,      5,      0,  },
	{      0,      4,      0,      0,      0,      0,      0,      5,      0,  },
	{      0,      0,     -5,     -5,     -5,     -5,     -5,      0,     -5,  },
	{      0,      0,     -9,     -9,     -9,     -9,     -9,      0,     -9,  },
	{      0,      0,     -8,     -8,     -8,     -8,     -8,      0,     -8,  },
	{      0,      0,     -7,     -7,     -7,      9,     10,      0,     -7,  },
	{      0,      0,     -6,     -6,     -6,      9,     10,      0,     -6,  },
	{     -1,     -1,      0,      0,      0,      0,      0,     -1,      0,  },
  };

public void parse() {
  YYStack  yystack = new YYStack();
  yystack.push(0);

  // Fetch until error (throw) or accept (return)
  while(true) {
    // Current lookahead
    YYSVal yysval = new YYSVal();
    int    yytok  = nextToken(yysval);

    if(yytok <  YYINTERN)  yytok = yyintern[yytok];
    if(yytok >= YYINTERN)  error("Unknown Token");
    if(yydebug) {
      System.err.println("Read " + yyterms[yytok]);
    }

    // Reduce until shift
    while(true) {
      final short  yyact = yyaction[yystack.top()][yytok];
      if(yyact == 0)  error("Syntax Error");
      if(yyact >  1) { // shift
        if(yydebug) {
          System.err.println("Push " + yyterms[yytok]);
        }
        yystack.push(yyact, yysval.val);
        break;
      }
      else {           // reduce (includes accept)
        int yylval = 0;
        final int  yyrno = (yyact < 0)? -yyact : 0;
        final int  yylen = yylength[yyrno];

        if(yydebug) {
          System.err.println("Reduce by " + yyrules[yyrno]);
        }
        switch(yyrno) { // do semantic actions
        case 0:         // accept
          return;
case 3: {
 // Single Equation
	  System.out.println("= " + yystack.peek(yylen - 1));
	
break;
}
case 4: {
 yylval = yystack.peek(yylen - 1); 
break;
}
case 5: {
 yylval = yystack.peek(yylen - 2); 
break;
}
case 6: {
 yylval = yystack.peek(yylen - 1) + yystack.peek(yylen - 3); 
break;
}
case 7: {
 yylval = yystack.peek(yylen - 1) - yystack.peek(yylen - 3); 
break;
}
case 8: {
 yylval = yystack.peek(yylen - 1) * yystack.peek(yylen - 3); 
break;
}
case 9: {
 yylval = yystack.peek(yylen - 1) / yystack.peek(yylen - 3); 
break;
}
        }
        
        yystack.pop(yylen);
        yystack.push(yygoto[yystack.top()][yylhs[yyrno]], yylval);
      }
    }
  }
}
}
