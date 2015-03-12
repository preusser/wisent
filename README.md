# The wisent Parser Generator #
Wisent is an LALR(1) parser generator with pluggable front- and
backends. It generates object-oriented code with clean namespace
separation. A yacc-like frontend is currently accompanied by a C++ and
a Java backend. The tool itself is implemented in C++.

## Platform
- independent (source)
- GNU make, POSIX shell (distributed build scripts)

## License

[GNU General Public License (GPL)][http://www.gnu.org/licenses/gpl.html]

## Quick Start

1. Clone repository: `git clone https://github.com/preusser/wisent.git`
2. Change into directory 'wisent': `cd wisent`
3. Build the parser generator: `make wisent`
4. Test the provided Java example (see also examples/Calc.howto):
    + `cd examples`
    + `../wisent -bjava Calc`
    + `javac Calc.java`
    + `java Calc`
