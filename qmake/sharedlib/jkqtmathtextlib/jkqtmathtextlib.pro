# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtmathtextlib
CONFIG (debug, debug|release): TARGET = jkqtmathtextlib_debug

TEMPLATE = lib
CONFIG+=dll
CONFIG += create_prl

win32 {
    DEFINES += JKQTP_LIB_IN_DLL
    DEFINES += JKQTP_LIB_EXPORT_LIBRARY
}

include(../../lib/jkqtmathtext.pri)
include(../../lib/jkqtpcommon.pri)

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
