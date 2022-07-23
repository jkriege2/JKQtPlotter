# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtplotterlib
CONFIG (debug, debug|release): TARGET = jkqtplotterlib_debug

TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

#DEFINES += JKQTBP_AUTOTIMER

include(../../../lib/jkqtpcommon.pri)
include(../../../lib/jkqtplotter.pri)
include(../../../lib/jkqtmathtext.pri)
include(../../../lib/jkqtfastplotter.pri)

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX



include(../../../lib/jkqtplotter_pch.pri)
precompile_header:!isEmpty(PRECOMPILED_HEADER) {
  DEFINES += USING_PCH
}
