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
               $$PWD/jkqtplotter/jkqtpgraphsbase.h \
               $$PWD/jkqtplotter/jkqtpgraphsbaseerrors.h \
               $$PWD/jkqtplotter/jkqtpgraphsbasestylingmixins.h \
               $$PWD/jkqtplotter/jkqtplotter.h \
               $$PWD/jkqtplotter/jkqtplotterstyle.h \
               $$PWD/jkqtplotter/jkqtpkeystyle.h \
               $$PWD/jkqtplotter/jkqtpbaseplotterstyle.h \
               $$PWD/jkqtplotter/jkqtpcoordinateaxes.h \
               $$PWD/jkqtplotter/jkqtpcoordinateaxesstyle.h \
               $$PWD/jkqtplotter/jkqtpgraphsbasestyle.h \
               $$PWD/jkqtplotter/jkqtpimagetools.h \
               $$PWD/jkqtplotter/graphs/jkqtpboxplot.h \
               $$PWD/jkqtplotter/graphs/jkqtpboxplotbase.h \
               $$PWD/jkqtplotter/graphs/jkqtpboxplotstylingmixins.h \
               $$PWD/jkqtplotter/graphs/jkqtpevaluatedfunctionbase.h \
               $$PWD/jkqtplotter/graphs/jkqtpevaluatedfunction.h \
               $$PWD/jkqtplotter/graphs/jkqtpfilledcurve.h \
               $$PWD/jkqtplotter/graphs/jkqtpgeometric.h \
               $$PWD/jkqtplotter/graphs/jkqtpgeoannotations.h \
               $$PWD/jkqtplotter/graphs/jkqtpgeobase.h \
               $$PWD/jkqtplotter/graphs/jkqtpgeolines.h \
               $$PWD/jkqtplotter/graphs/jkqtpgeoshapes.h \
               $$PWD/jkqtplotter/graphs/jkqtpimage.h \
               $$PWD/jkqtplotter/graphs/jkqtpimpulses.h \
               $$PWD/jkqtplotter/graphs/jkqtpparsedfunction.h \
               $$PWD/jkqtplotter/graphs/jkqtppeakstream.h \
               $$PWD/jkqtplotter/graphs/jkqtpsinglecolumnsymbols.h \
               $$PWD/jkqtplotter/graphs/jkqtpimageoverlays.h \
               $$PWD/jkqtplotter/graphs/jkqtpcontour.h \
               $$PWD/jkqtplotter/graphs/jkqtpimagergb.h \
               $$PWD/jkqtplotter/graphs/jkqtpviolinplot.h \
               $$PWD/jkqtplotter/graphs/jkqtpviolinplotstylingmixins.h \
               $$PWD/jkqtplotter/graphs/jkqtpstatisticsadaptors.h \
               $$PWD/jkqtplotter/graphs/jkqtpscatter.h \
               $$PWD/jkqtplotter/graphs/jkqtprange.h \
               $$PWD/jkqtplotter/graphs/jkqtpspecialline.h \
               $$PWD/jkqtplotter/graphs/jkqtpbarchartbase.h \
               $$PWD/jkqtplotter/graphs/jkqtpbarchart.h \
               $$PWD/jkqtplotter/graphs/jkqtpevaluatedparametriccurve.h \
               $$PWD/jkqtplotter/gui/jkqtpcomboboxes.h \
               $$PWD/jkqtplotter/gui/jkqtpenhancedspinboxes.h \
               $$PWD/jkqtplotter/gui/jkqtpenhancedtableview.h \
               $$PWD/jkqtplotter/gui/jkqtpgraphsmodel.h \
               $$PWD/jkqtplotter/gui/jkvanishqtoolbar.h



    SOURCES += \
               $$PWD/jkqtplotter/jkqtptools.cpp \
               $$PWD/jkqtplotter/jkqtpbaseelements.cpp \
               $$PWD/jkqtplotter/jkqtpbaseplotter.cpp \
               $$PWD/jkqtplotter/jkqtpdatastorage.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbase.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbaseerrors.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbasestylingmixins.cpp \
               $$PWD/jkqtplotter/jkqtpimagetools.cpp \
               $$PWD/jkqtplotter/jkqtplotter.cpp \
               $$PWD/jkqtplotter/jkqtplotterstyle.cpp \
               $$PWD/jkqtplotter/jkqtpkeystyle.cpp \
               $$PWD/jkqtplotter/jkqtpbaseplotterstyle.cpp \
               $$PWD/jkqtplotter/jkqtpcoordinateaxes.cpp \
               $$PWD/jkqtplotter/jkqtpcoordinateaxesstyle.cpp \
               $$PWD/jkqtplotter/jkqtpgraphsbasestyle.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpboxplot.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpboxplotbase.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpboxplotstylingmixins.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpevaluatedfunctionbase.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpevaluatedfunction.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpfilledcurve.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpgeometric.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpgeoannotations.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpgeobase.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpgeolines.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpgeoshapes.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpimage.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpimpulses.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpparsedfunction.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtppeakstream.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpsinglecolumnsymbols.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpimageoverlays.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpcontour.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpimagergb.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpviolinplot.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpviolinplotstylingmixins.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpstatisticsadaptors.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpscatter.cpp \
               $$PWD/jkqtplotter/graphs/jkqtprange.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpspecialline.cpp \
               $$PWD/jkqtplotter/graphs/jkqtpbarchartbase.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpbarchart.cpp  \
               $$PWD/jkqtplotter/graphs/jkqtpevaluatedparametriccurve.cpp  \
               $$PWD/jkqtplotter/gui/jkqtpcomboboxes.cpp \
               $$PWD/jkqtplotter/gui/jkqtpenhancedspinboxes.cpp \
               $$PWD/jkqtplotter/gui/jkqtpenhancedtableview.cpp \
               $$PWD/jkqtplotter/gui/jkqtpgraphsmodel.cpp \
               $$PWD/jkqtplotter/gui/jkvanishqtoolbar.cpp


    RESOURCES += $$PWD/jkqtplotter/resources/jkqtpbaseplotter.qrc \
                 $$PWD/jkqtplotter/resources/jkqtpstyles.qrc

    QT += xml svg opengl

}
