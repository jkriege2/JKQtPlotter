
include($$PWD/common.pri)


HEADERS += $$PWD/jkqtmathtext/jkqtmathtext.h

SOURCES += $$PWD/jkqtmathtext/jkqtmathtext.cpp


include($$PWD/jkqtplotterressources/math_fonts/xits.pri)
DEFINES += AUTOLOAD_XITS_FONTS

#win32:LIBS += -lgdi32
