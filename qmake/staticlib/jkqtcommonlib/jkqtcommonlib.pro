# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtcommonlib
CONFIG (debug, debug|release): TARGET = jkqtcommonlib_debug

TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
