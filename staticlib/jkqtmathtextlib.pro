TARGET = jkqtmathtextlib
CONFIG (debug, debug|release): TARGET = jkqtmathtextlib_debug

TEMPLATE = lib
CONFIG+=staticlib
CONFIG += create_prl

include(../lib/jkqtmathtext.pri)
