TARGET = jkqtphighrestimerlib
CONFIG (debug, debug|release): TARGET = jkqtphighrestimerlib_debug

TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

HEADERS += \
    ../../lib/jkqtplottertools/jkqtphighrestimer.h

SOURCES += \
    ../../lib/jkqtplottertools/jkqtphighrestimer.cpp

