

QMAKE_CXXFLAGS += -fexceptions

HEADERS += $$PWD/jkqtplotter/jkqtpbaseplotter.h \
           $$PWD/jkqtplotter/jkqtpdatastorage.h \
           $$PWD/jkqtplotter/jkqtpbasegraphs.h \
           $$PWD/jkqtplotter/jkqtpelements.h \
           $$PWD/jkqtplotter/jkqtpbaseelements.h \
           $$PWD/jkqtplotter/jkqtplotter.h \
           $$PWD/jkqtplottertools/jkqtptools.h \
           $$PWD/jkqtplottertools/jkqttools.h \
           $$PWD/jkqtplotter/jkqtpimageelements.h \
           $$PWD/jkqtplotter/jkqtpimagetools.h \
           $$PWD/jkqtplotter/jkqtpbarchartelements.h \
           $$PWD/jkqtplotter/jkqtpboxplotelements.h \
           $$PWD/jkqtplotter/jkqtpevaluatedfunctionelements.h \
           $$PWD/jkqtplotter/jkqtpfilledcurveelements.h \
           $$PWD/jkqtplotter/jkqtpimpulseselements.h \
           $$PWD/jkqtplotter/jkqtpparsedfunctionelements.h \
           $$PWD/jkqtplotter/jkqtpoverlayelements.h \
           $$PWD/jkqtplotter/jkqtpgeoelements.h \
           $$PWD/jkqtplottertools/jkqtpmathparser.h \
           $$PWD/jkqtplottertools/jkqtp_imexport.h \
           $$PWD/jkqtplotter/jkqtpplotsmodel.h \
           $$PWD/jkqtmathtext/jkqtmathtext.h \
           $$PWD/jkqtplottertools/jkqtphighrestimer.h


SOURCES += $$PWD/jkqtplotter/jkqtpbaseplotter.cpp \
           $$PWD/jkqtplotter/jkqtpdatastorage.cpp \
           $$PWD/jkqtplotter/jkqtpbasegraphs.cpp \
           $$PWD/jkqtplotter/jkqtpelements.cpp \
           $$PWD/jkqtplotter/jkqtpbaseelements.cpp \
           $$PWD/jkqtplotter/jkqtplotter.cpp \
           $$PWD/jkqtplottertools/jkqtptools.cpp \
           $$PWD/jkqtplottertools/jkqttools.cpp  \
           $$PWD/jkqtplotter/jkqtpimageelements.cpp  \
           $$PWD/jkqtplotter/jkqtpimagetools.cpp  \
           $$PWD/jkqtplotter/jkqtpbarchartelements.cpp  \
           $$PWD/jkqtplotter/jkqtpboxplotelements.cpp  \
           $$PWD/jkqtplotter/jkqtpevaluatedfunctionelements.cpp  \
           $$PWD/jkqtplotter/jkqtpfilledcurveelements.cpp  \
           $$PWD/jkqtplotter/jkqtpimpulseselements.cpp  \
           $$PWD/jkqtplotter/jkqtpparsedfunctionelements.cpp  \
           $$PWD/jkqtplotter/jkqtpoverlayelements.cpp  \
           $$PWD/jkqtplotter/jkqtpgeoelements.cpp  \
           $$PWD/jkqtplottertools/jkqtpmathparser.cpp \
           $$PWD/jkqtplotter/jkqtpplotsmodel.cpp \
           $$PWD/jkqtmathtext/jkqtmathtext.cpp \
           $$PWD/jkqtplottertools/jkqtphighrestimer.cpp


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
