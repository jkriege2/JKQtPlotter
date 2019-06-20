# JKQTPlotter: Build as a STATIC library

This directory contains Projects that can be used to build a static library of JKQTPlotter (i.e. the contents of the directory [`../lib`](../lib)

## QMake

### Build shared libraries
There are several `.PRO`-files, that can be used to build the full library, or a limited subsets. They will produce a static link libraries that you can include into your projects, e.g. with the following QMake-snippet:

```.qmake
# include JKQTPlotter library
DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/qmake/staticlib/jkqtplotterlib <PATHTOJKQTPLOTTERDIR>/lib
INCLUDEPATH += <PATHTOJKQTPLOTTERDIR>/lib
CONFIG (debug, debug|release) {
    DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/qmake/staticlib/jkqtplotterlib/debug
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/qmake/staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/qmake/staticlib/jkqtplotterlib/release
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/qmake/staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
```

This snippet assumes that you built the libraries with the provided `.PRO`-files.



