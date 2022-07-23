# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtcommonlib
CONFIG (debug, debug|release): TARGET = jkqtcommonlib_debug

TEMPLATE = lib
CONFIG += dll
CONFIG += create_prl

win32 {
    DEFINES += JKQTCOMMON_LIB_IN_DLL
    DEFINES += JKQTCOMMON_LIB_EXPORT_LIBRARY
}

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
