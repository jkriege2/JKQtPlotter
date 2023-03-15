# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtmathlib
CONFIG (debug, debug|release): TARGET = jkqtmathlib_debug

TEMPLATE = lib
CONFIG += dll
CONFIG += create_prl

win32 {
    DEFINES += jkqtmath_LIB_IN_DLL
    DEFINES += jkqtmath_LIB_EXPORT_LIBRARY
}

include(../../../lib/jkqtcommon.pri)

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
