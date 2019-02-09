# JKQTPlotter: Build as a SHARED library

This directory contains Projects that can be used to build a shared library of JKQTPlotter (i.e. the contents of the directory [`../lib`](../lib)

## QMake

### Build shared libraries
There are several `.PRO`-files, that can be used to build the full library, or a limited subsets. They will produce a dynamic link library that you can include into your projects, e.g. with the following QMake-snippet:

```.qmake
# include JKQTPlotter library
DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/lib <PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib
INCLUDEPATH += <PATHTOJKQTPLOTTERDIR>/lib
CONFIG (debug, debug|release) {
    # ensure that DLLs are copied to the output directory
    install_jkqtplotter_dll.files = <PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib/debug/jkqtplotterlib_debug.*
    install_jkqtplotter_dll.path = $$OUT_PWD
    INSTALLS += install_jkqtplotter_dll
    # link agains DLLs
    DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib/debug
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    # ensure that DLLs are copied to the output directory
    install_jkqtplotter_dll.files = <PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib/release/jkqtplotterlib.*
    install_jkqtplotter_dll.path = $$OUT_PWD
    INSTALLS += install_jkqtplotter_dll
    DEPENDPATH += <PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib/release
    # link agains DLLs
    LIBS += -L<PATHTOJKQTPLOTTERDIR>/sharedlib/jkqtplotterlib/release -ljkqtplotterlib
}
```

This snippet assumes that you built the libraries with the provided `.PRO`-files.

*Note* that you will have to run a deployment step `make install` before running your executable, so the shared libararies are actually copied to the output directory (see `INSTALLS + ...` above).



