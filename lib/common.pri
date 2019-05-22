
!win32-msvc* {
    QMAKE_CXXFLAGS += -fexceptions
} else {
    QMAKE_CXXFLAGS += /EHsc 
    # /std:c++14
    # To enable M_PI, M_E,...
    DEFINES += _USE_MATH_DEFINES
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    DEFINES += _CRT_NO_VA_START_VALIDATION
}

HEADERS += $$PWD/jkqtcommon/jkqtp_imexport.h \
           $$PWD/jkqtcommon/jkqtptools.h \
           $$PWD/jkqtcommon/jkqtptoolsdebugging.h \
           $$PWD/jkqtcommon/jkqtpcommonmathtools.h \
           $$PWD/jkqtcommon/jkqtpalgorithms.h \
           $$PWD/jkqtcommon/jkqtpstringtools.h


SOURCES += $$PWD/jkqtcommon/jkqtptools.cpp \
           $$PWD/jkqtcommon/jkqtptoolsdebugging.cpp \
           $$PWD/jkqtcommon/jkqtpcommonmathtools.cpp \
           $$PWD/jkqtcommon/jkqtpalgorithms.cpp \
           $$PWD/jkqtcommon/jkqtpstringtools.cpp

INCLUDEPATH += $$PWD 

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11
