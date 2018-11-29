# source code for this simple demo
SOURCES = jkqtplotter_simpletest_logaxes.cpp 

# configure Qt
CONFIG += qt
QT += core gui svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_logaxes

# include JKQtPlotter source code
include(../../lib/jkqtplotter.pri)
include(../../lib/jkqtplotterressources/math_fonts/xits.pri)
DEFINES += AUTOLOAD_XITS_FONTS
DEFINES += USE_XITS_FONTS

# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER
