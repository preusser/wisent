# The wisent Parser Generator #
Wisent is an LALR(1) parser generator with pluggable front- and
backends. It generates object-oriented code with clean namespace
separation. A yacc-like frontend is currently accompanied by a C++ and
a Java backend. The tool itself is implemented in C++.

## Platform
- independent (source)
- GNU make, POSIX shell (distributed build scripts)

## License

[GNU General Public License (GPL)](http://www.gnu.org/licenses/gpl.html)

## Build Requirements
Building wisent requires the lexer tool `flex`.

## Quick Start

1. Clone repository: `git clone https://github.com/preusser/wisent.git`
2. Change into directory 'wisent': `cd wisent`
3. Build the parser generator: `make wisent`
4. Test the provided Java example (see also examples/Calc.howto):
    + `cd examples`
    + `../wisent -bjava Calc`
    + `javac Calc.java`
    + `java Calc`

## Documentation

Please, refer to the technical report
[TUD-FI04-11](http://www.qucosa.de/fileadmin/data/qucosa/documents/9868/tud_TB_2004-11.pdf).

## References

Besides the [shipped examples](examples/), the parser for the wisent grammar itself in
implemented by a [wisent grammar](src/frontend/wisent/WP.ypp). Other use examples can
be found in the following projects:

Project | Grammar
--------|--------
[Q2D](https://github.com/fer-rum/q2d) | [Boolean Expression Parser](https://github.com/fer-rum/q2d/blob/master/interfaces/quantor/Quantorizer.ypp)
[Q27](https://github.com/preusser/q27)| [Range Specification Parser](https://github.com/preusser/q27/blob/master/src/cpp/range/RangeParser.ypp)
