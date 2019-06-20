# source code for this simple demo
SOURCES = rgbimageplot_opencv.cpp 

# configure Qt
CONFIG += link_prl qt
QT += core gui xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = rgbimageplot_opencv

exampleimg.files=$$PWD/example.bmp
exampleimg.path=$$OUT_PWD


# link agains OpenCV-3.4.1
#INCLUDEPATH += $$PWD/../../../OpenCV-3.4.1/include/
#LIBS += -L$$PWD/../../../OpenCV-3.4.1/bin/ -llibopencv_core341 -llibopencv_imgcodecs341
#opencvdlls.files=$$PWD/../../../OpenCV-3.4.1/bin/*.dll
#opencvdlls.path=$$OUT_PWD

# link agains OpenCV-4
INCLUDEPATH += $$PWD/../../../OpenCV-4.0.1/include/
LIBS += -L$$PWD/../../../OpenCV-4.0.1/x64/mingw/bin -L$$PWD/../../../OpenCV-4.0.1/x64/mingw/lib -llibopencv_core400.dll -llibopencv_imgcodecs400.dll
opencvdlls.files=$$PWD/../../../OpenCV-4.0.1/x64/mingw/bin/*.dll
opencvdlls.path=$$OUT_PWD

INCLUDEPATH+=../../lib

CONFIG (debug, debug|release) {
    LIBS += -L../../qmake/staticlib/jkqtplotterlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../qmake/staticlib/jkqtplotterlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

INSTALLS += opencvdlls exampleimg

win32-msvc*: DEFINES += _USE_MATH_DEFINES
win32-msvc*: DEFINES += NOMINMAX
