TARGET = jkqtmathtextlib
CONFIG (debug, debug|release): TARGET = jkqtmathtextlib_debug

TEMPLATE = lib
CONFIG+=dll

include(../lib/jkqtmathtext.pri)
