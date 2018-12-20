

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

HEADERS += $$PWD/jkqtfastplotter/jkqtfastplotter.h \
           $$PWD/jkqtplottertools/jkqtptools.h \
           $$PWD/jkqtplottertools/jkqtphighrestimer.h \
           $$PWD/jkqtplottertools/jkqtp_imexport.h


SOURCES += $$PWD/jkqtfastplotter/jkqtfastplotter.cpp \
           $$PWD/jkqtplottertools/jkqtptools.cpp \
           $$PWD/jkqtplottertools/jkqtphighrestimer.cpp


RESOURCES += $$PWD/jkqtplotterressources/jkqtpbaseplotter.qrc

INCLUDEPATH += $$PWD 

QT += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

