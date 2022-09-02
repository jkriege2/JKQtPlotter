TARGET = jkqtplotter_doc_imagegenerator
TEMPLATE = app

SOURCES += jkqtplotter_doc_imagegenerator.cpp

CONFIG += link_prl qt
QT += core gui 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


DEPENDPATH += ../../lib  ../../qmake/staticlib/jkqtplotterlib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    DEPENDPATH += ../../qmake/staticlib/jkqtplotterlib/debug
    LIBS += -L../../qmake/staticlib/jkqtplotterlib/debug -L../../../qmake/staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    DEPENDPATH += ../../qmake/staticlib/jkqtplotterlib/release
    LIBS += -L../../qmake/staticlib/jkqtplotterlib/release -L../../../qmake/staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
