# JKQtPlotter
## LIB subdirectory

This directory contains all files necessary to build a library with the JKQtPlotter and JKQtFastPlotter classes inside. There are several ways to add these to your program:

### QMake
#### simply include all necessary files
If you use QMake and simply want to include all necessary files into your project, include one of these `PRI`-files into your QMake Project:
- `jkqtplotter.pri` contains all files in this library, including `JKQtFastPlotter` and `JKQtMathText`
- `jkqtfastplotter.pri` contains only those files from this directory which are necessary to build `JKQtFastPlotter`
- `jkqtmathtext.pri` contains only those files from this directory which are necessary to build `JKQtMathText`

#### Build (static) libraries
There are also `.PRO`-files with the same names as the `.PRI`-files above, that can be used to build the full library, or a limited subset. They will produce a static link library that you can include into your projects, e.g. with the following QMake-snippet:

```qmake
# include JKQtPlotter library
DEPENDPATH += . <PATHTOJKQTPLOTTERDIR>/lib
INCLUDEPATH += <PATHTOJKQTPLOTTERDIR>/lib
CONFIG (debug, debug|release):LIBS += -L<PATHTOJKQTPLOTTERDIR>/lib/debug -ljkqtplotterlib
CONFIG (release):LIBS += -L<PATHTOJKQTPLOTTERDIR>/lib/release -ljkqtplotterlib
```

This snippet assumes that you built the libraries with the provided `.PRO`-files.