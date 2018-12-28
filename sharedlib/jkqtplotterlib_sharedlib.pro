TARGET = jkqtplotterlib
CONFIG (debug, debug|release): TARGET = jkqtplotterlib_debug

TEMPLATE = lib
CONFIG += dll

include(../lib/jkqtplotter.pri)
