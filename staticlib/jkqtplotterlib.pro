TARGET = jkqtplotterlib
CONFIG (debug, debug|release): TARGET = jkqtplotterlib_debug

TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

include(../lib/jkqtplotter.pri)
