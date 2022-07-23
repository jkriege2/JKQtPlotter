# include guard against multiple inclusion
isEmpty(JKQTP_COMMON_STATISCTICS_AND_MATH_PRI_INCLUDED) {
    JKQTP_COMMON_STATISCTICS_AND_MATH_PRI_INCLUDED = 1

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

    HEADERS += $$PWD/jkqtcommon_statistics_and_math/jkqtcommon_statistics_and_math_imexport.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpalgorithms.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtplinalgtools.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpmathparser.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtparraytools.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatisticstools.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatbasics.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstathistogram.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatkde.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatregression.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatpoly.h \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatgrouped.h



    SOURCES += $$PWD/jkqtcommon_statistics_and_math/jkqtpalgorithms.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtplinalgtools.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpmathparser.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtparraytools.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatbasics.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstathistogram.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatkde.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatregression.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatpoly.cpp \
               $$PWD/jkqtcommon_statistics_and_math/jkqtpstatgrouped.cpp


    INCLUDEPATH += $$PWD

    QT += core gui
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

    CONFIG += c++11
}
