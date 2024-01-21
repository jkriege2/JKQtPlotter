# include guard against multiple inclusion
isEmpty(JKQTP_COMMON_PRI_INCLUDED) {
    JKQTP_COMMON_PRI_INCLUDED = 1

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
        # fix errors with min()/max() macros from windows
        DEFINES += NOMINMAX
    }

    HEADERS += $$PWD/jkqtcommon/jkqtcommon_imexport.h \
               $$PWD/jkqtcommon/jkqtpdebuggingtools.h \
               $$PWD/jkqtcommon/jkqtpmathtools.h \
               $$PWD/jkqtcommon/jkqtpstringtools.h  \
               $$PWD/jkqtcommon/jkqtpcodestructuring.h \
               $$PWD/jkqtcommon/jkqtpbasicimagetools.h \
               $$PWD/jkqtcommon/jkqtpgeometrytools.h \
               $$PWD/jkqtcommon/jkqtpdrawingtools.h \
               $$PWD/jkqtcommon/jkqtpenhancedpainter.h \
               $$PWD/jkqtcommon/jkqtphighrestimer.h \
               $$PWD/jkqtcommon/jkqttools.h \
               $$PWD/jkqtcommon/jkqtpicons.h \
               $$PWD/jkqtcommon/jkqtpcsstools.h \
               $$PWD/jkqtcommon/jkqtpcachingtools.h \
               $$PWD/jkqtcommon/jkqtpconcurrencytools.h \
               $$PWD/jkqtcommon/jkqtpexpected.h



    SOURCES += $$PWD/jkqtcommon/jkqtpdebuggingtools.cpp \
               $$PWD/jkqtcommon/jkqtpmathtools.cpp \
               $$PWD/jkqtcommon/jkqtpstringtools.cpp  \
               $$PWD/jkqtcommon/jkqtpcodestructuring.cpp \
               $$PWD/jkqtcommon/jkqtpbasicimagetools.cpp \
               $$PWD/jkqtcommon/jkqtpgeometrytools.cpp \
               $$PWD/jkqtcommon/jkqtpdrawingtools.cpp \
               $$PWD/jkqtcommon/jkqtpenhancedpainter.cpp \
               $$PWD/jkqtcommon/jkqtphighrestimer.cpp \
               $$PWD/jkqtcommon/jkqttools.cpp \
               $$PWD/jkqtcommon/jkqtpicons.cpp \
               $$PWD/jkqtcommon/jkqtpcsstools.cpp \
               $$PWD/jkqtcommon/jkqtpcachingtools.cpp \
               $$PWD/jkqtcommon/jkqtpconcurrencytools.cpp 


    INCLUDEPATH += $$PWD

    QT += core gui
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

    CONFIG += c++11
}
