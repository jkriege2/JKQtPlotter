TARGET = jkqtfastplotterlib
CONFIG (debug, debug|release): TARGET = jkqtfastplotterlib_debug

TEMPLATE = lib
CONFIG+=dll

include(../lib/jkqtfastplotter.pri)
