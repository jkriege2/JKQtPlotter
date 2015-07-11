
LIBS +=


SOURCES = jkqtplot_test.cpp \
          ../../jkqtpbaseplotter.cpp \
          ../../jkqtpdatastorage.cpp \
          ../../jkqtplotter.cpp \
          ../../jkqtptools.cpp \
          ../../jkqtpelements.cpp \
          ../../jkqtpbaseelements.cpp \
          ../../jkqtmathtext.cpp \
          ../../jkqtpgeoelements.cpp \
    ../../jkqtpimageelements.cpp \
    ../../jkqtpoverlayelements.cpp \
    testmain.cpp \
    ../../jkqtpparsedfunctionelements.cpp \
    ../../jkqtpmathparser.cpp \
    ../../jkqtpimagetools.cpp \
QTeXEngine/src/QTeXPaintEngine.cpp \
    QTeXEngine/src/QTeXPaintDevice.cpp \

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

HEADERS = ../../jkqtpbaseplotter.h \
          ../../jkqtpdatastorage.h \
          ../../jkqtplotter.h \
          ../../jkqtptools.h \
          ../../jkqtpelements.h \
          ../../jkqtpgeoelements.h \
          ../../jkqtpbaseelements.h \
          ../../jkqtmathtext.h \
    ../../jkqtpimageelements.h \
    ../../jkqtpimagetools.h \
    ../../jkqtpoverlayelements.h \
    testmain.h \
    ../../jkqtpparsedfunctionelements.h \
    ../../jkqtpmathparser.h \
    ../../jkqtp_imexport.h


INCLUDEPATH +=

UI_DIR = .uics
OBJECTS_DIR = .objs
MOC_DIR = .mocs
RCC_DIR = .rccs
DESTDIR = ./

FORMS =


RESOURCES = ../../jkqtpbaseplotter.qrc \
    jkqtplot_test.qrc
