# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtmathtextlib
CONFIG (debug, debug|release): TARGET = jkqtmathtextlib_debug

TEMPLATE = lib
CONFIG+=dll
CONFIG += create_prl

win32 {
    DEFINES += JKQTCOMMON_LIB_IN_DLL  JKQTMATHTEXT_LIB_IN_DLL 
    DEFINES += JKQTCOMMON_LIB_EXPORT_LIBRARY  JKQTMATHTEXT_LIB_EXPORT_LIBRARY 
}

include(../../../lib/jkqtmathtext.pri)
include(../../../lib/jkqtcommon.pri)

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
