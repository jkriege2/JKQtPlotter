# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtplotterlib
CONFIG (debug, debug|release): TARGET = jkqtplotterlib_debug

TEMPLATE = lib
CONFIG += dll
CONFIG += create_prl

win32 {
    DEFINES += JKQTCOMMON_LIB_IN_DLL JKQTFASTPLOTTER_LIB_IN_DLL JKQTMATHTEXT_LIB_IN_DLL JKQTPLOTTER_LIB_IN_DLL
    DEFINES += JKQTCOMMON_LIB_EXPORT_LIBRARY JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY JKQTMATHTEXT_LIB_EXPORT_LIBRARY JKQTPLOTTER_LIB_EXPORT_LIBRARY
}

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX


include(../../../lib/jkqtplotter.pri)
include(../../../lib/jkqtpcommon.pri)
include(../../../lib/jkqtmathtext.pri)
include(../../../lib/jkqtfastplotter.pri)


include(../../../lib/jkqtplotter_pch.pri)
precompile_header:!isEmpty(PRECOMPILED_HEADER) {
  DEFINES += USING_PCH
}
