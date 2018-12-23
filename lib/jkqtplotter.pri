
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

HEADERS += $$PWD/jkqtplotter/jkqtpbaseplotter.h \
           $$PWD/jkqtplotter/jkqtpdatastorage.h \
           $$PWD/jkqtplotter/jkqtpgraphsbase.h \
           $$PWD/jkqtplotter/jkqtpgraphs.h \
           $$PWD/jkqtplotter/jkqtpbaseelements.h \
           $$PWD/jkqtplotter/jkqtplotter.h \
           $$PWD/jkqtplottertools/jkqtptools.h \
           $$PWD/jkqtplottertools/jkqttools.h \
           $$PWD/jkqtplotter/jkqtpgraphsimage.h \
           $$PWD/jkqtplottertools/jkqtpimagetools.h \
           $$PWD/jkqtplotter/jkqtpgraphsbarchart.h \
           $$PWD/jkqtplotter/jkqtpgraphsboxplot.h \
           $$PWD/jkqtplotter/jkqtpgraphsevaluatedfunction.h \
           $$PWD/jkqtplotter/jkqtpgraphsfilledcurve.h \
           $$PWD/jkqtplotter/jkqtpgraphsimpulses.h \
           $$PWD/jkqtplotter/jkqtpgraphsparsedfunction.h \
           $$PWD/jkqtplotter/jkqtpelementsoverlay.h \
           $$PWD/jkqtplotter/jkqtpgraphsgeometric.h \
           $$PWD/jkqtplotter/jkqtpgraphspeakstream.h \
           $$PWD/jkqtplottertools/jkqtpmathparser.h \
           $$PWD/jkqtplottertools/jkqtp_imexport.h \
           $$PWD/jkqtplottergui/jkqtpgraphsmodel.h \
           $$PWD/jkqtplottergui/jkqtpcomboboxes.h \
           $$PWD/jkqtplottergui/jkqtpenhancedtableview.h \
           $$PWD/jkqtplottergui/jkqtpenhancedspinboxes.h \
           $$PWD/jkqtplottergui/jkvanishqtoolbar.h \
           $$PWD/jkqtmathtext/jkqtmathtext.h \
           $$PWD/jkqtplottertools/jkqtpdrawingtools.h \
           $$PWD/jkqtplottertools/jkqtphighrestimer.h \
           $$PWD/jkqtplottertools/jkqtpenhancedpainter.h


SOURCES += $$PWD/jkqtplotter/jkqtpbaseplotter.cpp \
           $$PWD/jkqtplotter/jkqtpdatastorage.cpp \
           $$PWD/jkqtplotter/jkqtpgraphsbase.cpp \
           $$PWD/jkqtplotter/jkqtpgraphs.cpp \
           $$PWD/jkqtplotter/jkqtpbaseelements.cpp \
           $$PWD/jkqtplotter/jkqtplotter.cpp \
           $$PWD/jkqtplottertools/jkqtptools.cpp \
           $$PWD/jkqtplottertools/jkqttools.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimage.cpp  \
           $$PWD/jkqtplottertools/jkqtpimagetools.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsbarchart.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsboxplot.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsevaluatedfunction.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsfilledcurve.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimpulses.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsparsedfunction.cpp  \
           $$PWD/jkqtplotter/jkqtpelementsoverlay.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsgeometric.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphspeakstream.cpp  \
           $$PWD/jkqtplottertools/jkqtpmathparser.cpp \
           $$PWD/jkqtplottergui/jkqtpgraphsmodel.cpp \
           $$PWD/jkqtplottergui/jkqtpcomboboxes.cpp \
           $$PWD/jkqtplottergui/jkqtpenhancedtableview.cpp \
           $$PWD/jkqtplottergui/jkqtpenhancedspinboxes.cpp \
           $$PWD/jkqtplottergui/jkvanishqtoolbar.cpp \
           $$PWD/jkqtmathtext/jkqtmathtext.cpp \
           $$PWD/jkqtplottertools/jkqtpdrawingtools.cpp \
           $$PWD/jkqtplottertools/jkqtphighrestimer.cpp \
           $$PWD/jkqtplottertools/jkqtpenhancedpainter.cpp


RESOURCES += $$PWD/jkqtplotterressources/jkqtpbaseplotter.qrc

INCLUDEPATH += $$PWD 

QT += core gui xml svg
#win32:LIBS += -lgdi32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

include($$PWD/jkqtplotterressources/math_fonts/xits.pri)
DEFINES += AUTOLOAD_XITS_FONTS
DEFINES += USE_XITS_FONTS

# for support of palettes stored as XML, use:
QT += xml
