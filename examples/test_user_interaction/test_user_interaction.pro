
# configure Qt
CONFIG += link_prl qt
CONFIG += link_prl c++11
QT += core gui xml  svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# output executable name
TARGET = test_user_interaction

# source code for this simple demo
SOURCES = test_user_interaction_main.cpp \
    test_user_interaction.cpp

HEADERS += \
    test_user_interaction.h

# include JKQTPlotter source code
DEPENDPATH += . ../../lib
INCLUDEPATH += ../../lib
CONFIG (debug, debug|release) {
    LIBS += -L../../staticlib/debug -ljkqtplotterlib_debug
} else {
    LIBS += -L../../staticlib/release -ljkqtplotterlib
}
message("LIBS = $$LIBS")

win32-msvc* {
    QMAKE_CXXFLAGS += /EHsc /std:c++14
    # To enable M_PI, M_E,...
    DEFINES += _USE_MATH_DEFINES
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    DEFINES += _CRT_NO_VA_START_VALIDATION
}


# here you can activate some debug options
#DEFINES += SHOW_JKQTPLOTTER_DEBUG
#DEFINES += JKQTBP_AUTOTIMER

