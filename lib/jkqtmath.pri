# include guard against multiple inclusion
isEmpty(JKQTP_MATH_PRI_INCLUDED) {
    JKQTP_MATH_PRI_INCLUDED = 1

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

    HEADERS += $$PWD/jkqtmath/jkqtmath_imexport.h \
               $$PWD/jkqtmath/jkqtpalgorithms.h \
               $$PWD/jkqtmath/jkqtplinalgtools.h \
               $$PWD/jkqtmath/jkqtpmathparser.h \
               $$PWD/jkqtmath/jkqtparraytools.h \
               $$PWD/jkqtmath/jkqtpstatisticstools.h \
               $$PWD/jkqtmath/jkqtpstatbasics.h \
               $$PWD/jkqtmath/jkqtpstathistogram.h \
               $$PWD/jkqtmath/jkqtpstatkde.h \
               $$PWD/jkqtmath/jkqtpstatregression.h \
               $$PWD/jkqtmath/jkqtpstatpoly.h \
               $$PWD/jkqtmath/jkqtpstatgrouped.h



    SOURCES += $$PWD/jkqtmath/jkqtpalgorithms.cpp \
               $$PWD/jkqtmath/jkqtplinalgtools.cpp \
               $$PWD/jkqtmath/jkqtpmathparser.cpp \
               $$PWD/jkqtmath/jkqtparraytools.cpp \
               $$PWD/jkqtmath/jkqtpstatbasics.cpp \
               $$PWD/jkqtmath/jkqtpstathistogram.cpp \
               $$PWD/jkqtmath/jkqtpstatkde.cpp \
               $$PWD/jkqtmath/jkqtpstatregression.cpp \
               $$PWD/jkqtmath/jkqtpstatpoly.cpp \
               $$PWD/jkqtmath/jkqtpstatgrouped.cpp


    INCLUDEPATH += $$PWD

    QT += core gui
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

    CONFIG += c++11
}
