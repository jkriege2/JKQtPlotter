
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

HEADERS += \
		   $$PWD/jkqtfastplotter/jkqtfastplotter.h \
           $$PWD/jkqtmathtext/jkqtmathtext.h \
           $$PWD/jkqtplotter/jkqtpbaseelements.h \
           $$PWD/jkqtplotter/jkqtpbaseplotter.h \
           $$PWD/jkqtplotter/jkqtpdatastorage.h \
           $$PWD/jkqtplotter/jkqtpelementsoverlay.h \
           $$PWD/jkqtplotter/jkqtpgraphs.h \
           $$PWD/jkqtplotter/jkqtpgraphsbarchart.h \
           $$PWD/jkqtplotter/jkqtpgraphsbase.h \
           $$PWD/jkqtplotter/jkqtpgraphsboxplot.h \
           $$PWD/jkqtplotter/jkqtpgraphsevaluatedfunction.h \
           $$PWD/jkqtplotter/jkqtpgraphsfilledcurve.h \
           $$PWD/jkqtplotter/jkqtpgraphsgeometric.h \
           $$PWD/jkqtplotter/jkqtpgraphsimage.h \
           $$PWD/jkqtplotter/jkqtpgraphsimpulses.h \
           $$PWD/jkqtplotter/jkqtpgraphsparsedfunction.h \
           $$PWD/jkqtplotter/jkqtpgraphspeakstream.h \
           $$PWD/jkqtplotter/jkqtplotter.h \
           $$PWD/jkqtplottergui/jkqtpcomboboxes.h \
           $$PWD/jkqtplottergui/jkqtpenhancedspinboxes.h \
           $$PWD/jkqtplottergui/jkqtpenhancedtableview.h \
           $$PWD/jkqtplottergui/jkqtpgraphsmodel.h \
           $$PWD/jkqtplottergui/jkvanishqtoolbar.h \
           $$PWD/jkqtplottertools/jkqtp_imexport.h \
           $$PWD/jkqtplottertools/jkqtpdrawingtools.h \
           $$PWD/jkqtplottertools/jkqtpenhancedpainter.h \
           $$PWD/jkqtplottertools/jkqtphighrestimer.h \
           $$PWD/jkqtplottertools/jkqtpimagetools.h \
           $$PWD/jkqtplottertools/jkqtpmathparser.h \
           $$PWD/jkqtplottertools/jkqtptools.h \
           $$PWD/jkqtplottertools/jkqttools.h 


SOURCES += \
		   $$PWD/jkqtfastplotter/jkqtfastplotter.cpp \
           $$PWD/jkqtmathtext/jkqtmathtext.cpp \
           $$PWD/jkqtplotter/jkqtpbaseelements.cpp \
           $$PWD/jkqtplotter/jkqtpbaseplotter.cpp \
           $$PWD/jkqtplotter/jkqtpdatastorage.cpp \
           $$PWD/jkqtplotter/jkqtpelementsoverlay.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphs.cpp \
           $$PWD/jkqtplotter/jkqtpgraphsbarchart.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsbase.cpp \
           $$PWD/jkqtplotter/jkqtpgraphsboxplot.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsevaluatedfunction.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsfilledcurve.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsgeometric.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimage.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimpulses.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsparsedfunction.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphspeakstream.cpp  \
           $$PWD/jkqtplotter/jkqtplotter.cpp \
           $$PWD/jkqtplottergui/jkqtpcomboboxes.cpp \
           $$PWD/jkqtplottergui/jkqtpenhancedspinboxes.cpp \
           $$PWD/jkqtplottergui/jkqtpenhancedtableview.cpp \
           $$PWD/jkqtplottergui/jkqtpgraphsmodel.cpp \
           $$PWD/jkqtplottergui/jkvanishqtoolbar.cpp \
           $$PWD/jkqtplottertools/jkqtpdrawingtools.cpp \
           $$PWD/jkqtplottertools/jkqtpenhancedpainter.cpp \
           $$PWD/jkqtplottertools/jkqtphighrestimer.cpp \
           $$PWD/jkqtplottertools/jkqtpimagetools.cpp  \
           $$PWD/jkqtplottertools/jkqtpmathparser.cpp \
           $$PWD/jkqtplottertools/jkqtptools.cpp \
           $$PWD/jkqtplottertools/jkqttools.cpp  


RESOURCES += $$PWD/jkqtplotterressources/jkqtpbaseplotter.qrc

INCLUDEPATH += $$PWD 

QT += core gui xml svg opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

include($$PWD/jkqtplotterressources/math_fonts/xits.pri)
DEFINES += AUTOLOAD_XITS_FONTS
DEFINES += USE_XITS_FONTS
