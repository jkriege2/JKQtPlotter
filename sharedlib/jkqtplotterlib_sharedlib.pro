TARGET = jkqtplotterlib
CONFIG (debug, debug|release): TARGET = jkqtplotterlib_debug

TEMPLATE = lib
CONFIG += dll
CONFIG += create_prl

win32 {
    DEFINES += LIB_IN_DLL
    DEFINES += LIB_EXPORT_LIBRARY
}

include(../lib/jkqtplotter.pri)
