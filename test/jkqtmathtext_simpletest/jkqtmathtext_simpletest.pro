TARGET = jkqtmathtext_simpletest
TEMPLATE = app

SOURCES += jkqtmathtext_simpletest.cpp

CONFIG += qt
QT += core gui xml  svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


DEPENDPATH += . ../../lib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release):LIBS += -L../../lib/debug -ljkqtplotterlib
CONFIG (release):LIBS += -L../../lib/release -ljkqtplotterlib
