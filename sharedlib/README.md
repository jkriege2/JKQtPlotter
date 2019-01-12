# JKQtPlotter: Build as a SHARED library

This directory contains Projects that can be used to build a shared library of JKQtPlotter (i.e. the contents of the directory [`../lib`](../lib)

## QMake

### Build shared libraries
There are several `.PRO`-files, that can be used to build the full library, or a limited subsets. They will produce a dynamic link library that you can include into your projects, e.g. with the following QMake-snippet:

```qmake
# include JKQtPlotter library
DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/lib
INCLUDEPATH += <PATHTOJKQTPLOTTERDIR>/lib
CONFIG (debug, debug|release) {
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/sharedlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/sharedlib/release -ljkqtplotterlib
}
```

This snippet assumes that you built the libraries with the provided `.PRO`-files.



