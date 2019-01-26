TARGET = jkqtfastplotterlib
CONFIG (debug, debug|release): TARGET = jkqtfastplotterlib_debug

TEMPLATE = lib
CONFIG+=staticlib
CONFIG += create_prl

include(../lib/jkqtfastplotter.pri)
