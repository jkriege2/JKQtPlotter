# source code for this simple demo
SOURCES = jkqtplotter_simpletest_rgbimageplot_opencv.cpp 

# configure Qt
CONFIG += qt
QT += core gui svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = jkqtplotter_simpletest_rgbimageplot_opencv

# include JKQtPlotter source code
include(../../lib/jkqtplotter.pri)
# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER


# link agains OpenCV-3.4.1
INCLUDEPATH += $$PWD/OpenCV-3.4.1/include/
LIBS += -L$$PWD/OpenCV-3.4.1/bin/ -llibopencv_core341 -llibopencv_imgcodecs341

# add OpenCV-interface to JKQTPdatastore
DEFINES += JKQTPLOTTER_OPENCV_INTERFACE
