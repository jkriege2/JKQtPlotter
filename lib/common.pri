
!win32-msvc* {
    QMAKE_CXXFLAGS += -fexceptions
} else {
    QMAKE_CXXFLAGS += /EHsc
    # To enable M_PI, M_E,...
    DEFINES += _USE_MATH_DEFINES
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    DEFINES += _CRT_NO_VA_START_VALIDATION
}

HEADERS += $$PWD/jkqtplottertools/jkqtp_imexport.h \
           $$PWD/jkqtplottertools/jkqtptools.h


SOURCES += $$PWD/jkqtplottertools/jkqtptools.cpp

INCLUDEPATH += $$PWD 

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
