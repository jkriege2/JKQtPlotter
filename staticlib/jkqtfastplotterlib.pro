# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtfastplotterlib
CONFIG (debug, debug|release): TARGET = jkqtfastplotterlib_debug

TEMPLATE = lib
CONFIG+=staticlib
CONFIG += create_prl

include(../lib/jkqtfastplotter.pri)
