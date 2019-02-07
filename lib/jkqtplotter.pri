# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS


include($$PWD/common.pri)


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
           $$PWD/jkqtplotter/jkqtpgraphsbaseerrors.h \
           $$PWD/jkqtplotter/jkqtpgraphsboxplot.h \
           $$PWD/jkqtplotter/jkqtpgraphsevaluatedfunction.h \
           $$PWD/jkqtplotter/jkqtpgraphsfilledcurve.h \
           $$PWD/jkqtplotter/jkqtpgraphsgeometric.h \
           $$PWD/jkqtplotter/jkqtpgraphsimage.h \
           $$PWD/jkqtplotter/jkqtpgraphsimpulses.h \
           $$PWD/jkqtplotter/jkqtpgraphsparsedfunction.h \
           $$PWD/jkqtplotter/jkqtpgraphspeakstream.h \
           $$PWD/jkqtplotter/jkqtpgraphssinglecolumnsymbols.h \
           $$PWD/jkqtplotter/jkqtpgraphsimageoverlays.h \
           $$PWD/jkqtplotter/jkqtpgraphscontour.h \
           $$PWD/jkqtplotter/jkqtpgraphsimagergb.h \
           $$PWD/jkqtplotter/jkqtplotter.h \
           $$PWD/jkqtplotter/jkqtplotterstyle.h \
           $$PWD/jkqtplotter/jkqtpbaseplotterstyle.h \ 
           $$PWD/jkqtplotter/jkqtpcoordinateaxes.h \
           $$PWD/jkqtplotter/jkqtpcoordinateaxesstyle.h \
           $$PWD/jkqtplottergui/jkqtpcomboboxes.h \
           $$PWD/jkqtplottergui/jkqtpenhancedspinboxes.h \
           $$PWD/jkqtplottergui/jkqtpenhancedtableview.h \
           $$PWD/jkqtplottergui/jkqtpgraphsmodel.h \
           $$PWD/jkqtplottergui/jkvanishqtoolbar.h \
           $$PWD/jkqtplottertools/jkqtpdrawingtools.h \
           $$PWD/jkqtplottertools/jkqtpenhancedpainter.h \
           $$PWD/jkqtplottertools/jkqtphighrestimer.h \
           $$PWD/jkqtplottertools/jkqtpimagetools.h \
           $$PWD/jkqtplottertools/jkqtpmathparser.h \
           $$PWD/jkqtplottertools/jkqttools.h \
    $$PWD/jkqtplotter/jkqtpkeystyle.h


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
           $$PWD/jkqtplotter/jkqtpgraphsbaseerrors.cpp \
           $$PWD/jkqtplotter/jkqtpgraphsboxplot.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsevaluatedfunction.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsfilledcurve.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsgeometric.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimage.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimpulses.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsparsedfunction.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphspeakstream.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphssinglecolumnsymbols.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimageoverlays.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphscontour.cpp  \
           $$PWD/jkqtplotter/jkqtpgraphsimagergb.cpp  \
           $$PWD/jkqtplotter/jkqtplotter.cpp \
           $$PWD/jkqtplotter/jkqtplotterstyle.cpp \
           $$PWD/jkqtplotter/jkqtpbaseplotterstyle.cpp \ 
           $$PWD/jkqtplotter/jkqtpcoordinateaxes.cpp \
           $$PWD/jkqtplotter/jkqtpcoordinateaxesstyle.cpp \
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
           $$PWD/jkqtplottertools/jkqttools.cpp \
    $$PWD/jkqtplotter/jkqtpkeystyle.cpp


RESOURCES += $$PWD/jkqtplotterressources/jkqtpbaseplotter.qrc \
             $$PWD/jkqtplotterressources/jkqtpstyles.qrc

QT += xml svg opengl

include($$PWD/jkqtplotterressources/math_fonts/xits.pri)
DEFINES += AUTOLOAD_XITS_FONTS
DEFINES += USE_XITS_FONTS
