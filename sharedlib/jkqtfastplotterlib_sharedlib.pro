# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtfastplotterlib
CONFIG (debug, debug|release): TARGET = jkqtfastplotterlib_debug

TEMPLATE = lib
CONFIG+=dll
CONFIG += create_prl

win32 {
    DEFINES += JKQTP_LIB_IN_DLL
    DEFINES += JKQTP_LIB_EXPORT_LIBRARY
}

include(../lib/jkqtfastplotter.pri)
