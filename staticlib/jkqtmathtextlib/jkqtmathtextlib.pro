# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS
TARGET = jkqtmathtextlib
CONFIG (debug, debug|release): TARGET = jkqtmathtextlib_debug

TEMPLATE = lib
CONFIG += staticlib
CONFIG += create_prl

include(../../lib/jkqtmathtext.pri)
include(../../lib/jkqtpcommon.pri)
