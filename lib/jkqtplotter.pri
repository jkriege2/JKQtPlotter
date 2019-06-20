# uncomment this line to prevent linking in of the XITS fonts
#DEFINES += NO_XITS_FONTS



isEmpty(JKQTP_PLOTTER_PRI_INCLUDED) {
    JKQTP_PLOTTER_PRI_INCLUDED = 1

    INCLUDEPATH += $PWD


    HEADERS += \
               $$PWD/jkqtplotter/jkqtptools.h \
               $$PWD/jkqtplotter/jkqtpbaseelements.h \
               $$PWD/jkqtplotter/jkqtpbaseplotter.h \
               $$PWD/jkqtplotter/jkqtpdatastorage.h \
               $$PWD/jkqtplotter/jkqtpelementsoverlay.h \
               $$PWD/jkqtplotter/jkqtpgraphsscatter.h \
               $$PWD/jkqtplotter/jkqtpgraphsrange.h \
               $$PWD/jkqtplotter/jkqtpgraphsspecialline.h \
               $$PWD/jkqtplotter/jkqtpgraphsbarchart.h \
               $$PWD/jkqtplotter/jkqtpgraphsbase.h \
               $$PWD/jkqtplotter/jkqtpgraphsbaseerrors.h \
               $$PWD/jkqtplotter/jkqtpgraphsbasestylingmixins.h \
               $$PWD/jkqtplotter/jkqtpgraphsboxplot.h \
               $$PWD/jkqtplotter/jkqtpgraphsboxplotstylingmixins.h \
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
               $$PWD/jkqtplotter/jkqtpgraphsviolinplot.h \
               $$PWD/jkqtplotter/jkqtpgraphsviolinplotstylingmixins.h \
               $$PWD/jkqtplotter/jkqtpgraphsstatisticsadaptors.h \
               $$PWD/jkqtplotter/jkqtplotter.h \
               $$PWD/jkqtplotter/jkqtplotterstyle.h \
               $$PWD/jkqtplotter/jkqtpkeystyle.h \
               $$PWD/jkqtplotter/jkqtpbaseplotterstyle.h \
               $$PWD/jkqtplotter/jkqtpcoordinateaxes.h \
               $$PWD/jkqtplotter/jkqtpcoordinateaxesstyle.h \
               $$PWD/jkqtplotter/gui/jkqtpcomboboxes.h \
               $$PWD/jkqtplotter/gui/jkqtpenhancedspinboxes.h \
               $$PWD/jkqtplotter/gui/jkqtpenhancedtableview.h \
               $$PWD/jkqtplotter/gui/jkqtpgraphsmodel.h \
               $$PWD/jkqtplotter/gui/jkvanishqtoolbar.h \
               $$PWD/jkqtplotter/jkqtpimagetools.h



    SOURCES += \
               $$PWD/jkqtplotter/jkqtptools.cpp \
               $$PWD/jkqtplotter/jkqtpbaseelements.cpp \
               $$PWD/jkqtplotter/jkqtpbaseplotter.cpp \
               $$PWD/jkqtplotter/jkqtpdatastorage.cpp \
               $$PWD/jkqtplotter/jkqtpelementsoverlay.cpp  \
               $$PWD/jkqtplotter/jkqtpgraphsscatter.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsrange.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsspecialline.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbarchart.cpp  \
               $$PWD/jkqtplotter/jkqtpgraphsbase.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbaseerrors.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbasestylingmixins.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsboxplot.cpp  \
               $$PWD/jkqtplotter/jkqtpgraphsboxplotstylingmixins.cpp  \
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
               $$PWD/jkqtplotter/jkqtpgraphsimagergb.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsviolinplot.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsviolinplotstylingmixins.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsstatisticsadaptors.cpp  \
               $$PWD/jkqtplotter/jkqtplotter.cpp \
               $$PWD/jkqtplotter/jkqtplotterstyle.cpp \
               $$PWD/jkqtplotter/jkqtpkeystyle.cpp \
               $$PWD/jkqtplotter/jkqtpbaseplotterstyle.cpp \
               $$PWD/jkqtplotter/jkqtpcoordinateaxes.cpp \
               $$PWD/jkqtplotter/jkqtpcoordinateaxesstyle.cpp \
               $$PWD/jkqtplotter/gui/jkqtpcomboboxes.cpp \
               $$PWD/jkqtplotter/gui/jkqtpenhancedspinboxes.cpp \
               $$PWD/jkqtplotter/gui/jkqtpenhancedtableview.cpp \
               $$PWD/jkqtplotter/gui/jkqtpgraphsmodel.cpp \
               $$PWD/jkqtplotter/gui/jkvanishqtoolbar.cpp \
               $$PWD/jkqtplotter/jkqtpimagetools.cpp


    RESOURCES += $$PWD/jkqtplotter/resources/jkqtpbaseplotter.qrc \
                 $$PWD/jkqtplotter/resources/jkqtpstyles.qrc

    QT += xml svg opengl

}
