# NOTE: This shouldn't be built as a SHADOW build
!equals(PWD, $${OUT_PWD}) {
    warning(This project should not be built as a shadow build!!!)
}

include(../../jkqtplotter.pri)

SOURCES += jkqtplot_test.cpp \
    testmain.cpp \
    QTeXEngine/src/QTeXPaintEngine.cpp \
    QTeXEngine/src/QTeXPaintDevice.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
} else {
    win32  {
        SOURCES += jkqtpemfengineadapter.cpp \
            EmfEngine/src/EmfPaintDevice.cpp \
            EmfEngine/src/EmfEngine.cpp

        HEADERS +=EmfEngine/src/EmfEngine.h \
            jkqtpemfengineadapter.h
    }
}

CONFIG += qt console

#QMAKE_LFLAGS += -s

win32: LIBS += -lgdi32

#QMAKE_CXXFLAGS += -O2

DEFINES -= -DQT_DLL
DEFINES +=
#SHOW_JKQTPLOTTER_DEBUG
#JKQTBP_AUTOTIMER

TARGET = jkqtplot_test

QT += core gui svg xml
win32:LIBS += -lgdi32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

#if not added by qmake:
DEFINES += QT_XML_LIB

HEADERS += testmain.h


INCLUDEPATH +=

UI_DIR = .uics
OBJECTS_DIR = .objs
MOC_DIR = .mocs
RCC_DIR = .rccs
DESTDIR = ./


RESOURCES += jkqtplot_test.qrc
