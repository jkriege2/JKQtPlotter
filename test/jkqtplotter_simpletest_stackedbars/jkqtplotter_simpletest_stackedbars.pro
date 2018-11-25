# source code for this simple demo
SOURCES = jkqtplotter_simpletest_stackedbars.cpp 

# configure Qt
CONFIG += qt
QT += core gui svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_stackedbars

# include JKQtPlotter source code
include(../../jkqtplotter.pri)
# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER
