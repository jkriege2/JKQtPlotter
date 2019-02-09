# source code for this simple demo
SOURCES = jkqtplotter_simpletest_imageplot_opencv.cpp 

# configure Qt
CONFIG += link_prl qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_imageplot_opencv

# add OpenCV-interface to JKQTPDatastore
DEFINES += JKQTPLOTTER_OPENCV_INTERFACE








# link agains OpenCV-3.4.1
INCLUDEPATH += $$PWD/OpenCV-3.4.1/include/
LIBS += -L$$PWD/OpenCV-3.4.1/bin/ -llibopencv_core341

CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")