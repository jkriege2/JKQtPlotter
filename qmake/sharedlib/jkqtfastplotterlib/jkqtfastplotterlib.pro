# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtfastplotterlib
CONFIG (debug, debug|release): TARGET = jkqtfastplotterlib_debug

TEMPLATE = lib
CONFIG+=dll
CONFIG += create_prl

win32 {
    DEFINES += JKQTCOMMON_LIB_IN_DLL JKQTFASTPLOTTER_LIB_IN_DLL  
    DEFINES += JKQTCOMMON_LIB_EXPORT_LIBRARY JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY  
}

include(../../../lib/jkqtfastplotter.pri)
include(../../../lib/jkqtcommon.pri)

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
