
SOURCES += jkqtplot_test.cpp \
    testmain.cpp \
    TestWidgetFunctionPlots.cpp \
    TestWidgetGeometry.cpp \
    TestWidgetGraphs.cpp \
    TestWidgetImages.cpp \
    TestWidgetLogGraphs.cpp \
    TestWidgetParamScatterPlots.cpp \
    TestWidgetPeaksPlots.cpp \
    TestWidgetRGBImages.cpp \
    TestWidgetBarcharts.cpp \
    TestWidgetContourPlots.cpp \
    TestWidgetEmptyPlot.cpp \
    QTeXEngine/src/QTeXPaintEngine.cpp \
    QTeXEngine/src/QTeXPaintDevice.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
} else {
    win32  {
        SOURCES += jkqtpemfengineadapter.cpp \
            EmfEngine/src/EmfPaintDevice.cpp \
            EmfEngine/src/EmfEngine.cpp \

        HEADERS +=EmfEngine/src/EmfEngine.h \
            jkqtpemfengineadapter.h

        LIBS += -lgdi32
    }
}


HEADERS += testmain.h \
    TestWidgetFunctionPlots.h \
    TestWidgetGeometry.h \
    TestWidgetGraphs.h \
    TestWidgetImages.h \
    TestWidgetLogGraphs.h \
    TestWidgetParamScatterPlots.h \
    TestWidgetPeaksPlots.h \
    TestWidgetRGBImages.h \
    TestWidgetBarcharts.h \
    TestWidgetContourPlots.h \
    TestWidgetEmptyPlot.h \
    QTeXEngine/src/QTeXEngine.h


RESOURCES += jkqtplot_test.qrc


win32-msvc*: DEFINES += _USE_MATH_DEFINES

TARGET = jkqtplot_test

# configure Qt
CONFIG += qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


# include JKQtPlotter source code
DEPENDPATH += $$PWD
#$$PWD/../../lib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")


# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER
