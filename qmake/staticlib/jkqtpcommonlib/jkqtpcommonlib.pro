# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtpcommonlib
CONFIG (debug, debug|release): TARGET = jkqtmathtextlib_debug

TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

include(../../../lib/jkqtpcommon.pri)

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
