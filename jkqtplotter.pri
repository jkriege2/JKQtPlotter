

QMAKE_CXXFLAGS += -fexceptions

HEADERS += $$PWD/jkqtpbaseplotter.h \
           $$PWD/jkqtpdatastorage.h \
           $$PWD/jkqtpelements.h \
           $$PWD/jkqtmathtext.h \
           $$PWD/jkqtpbaseelements.h \
           $$PWD/jkqtplotter.h \
           $$PWD/jkqtptools.h \
           $$PWD/jkqttools.h \
           $$PWD/jkqtpimageelements.h \
           $$PWD/jkqtpimagetools.h \
           $$PWD/jkqtpparsedfunctionelements.h \
           $$PWD/jkqtpoverlayelements.h \
           $$PWD/jkqtpgeoelements.h \
           $$PWD/jkqtpmathparser.h \
           $$PWD/jkqtp_imexport.h \
           $$PWD/jkqtpplotsmodel.h


SOURCES += $$PWD/jkqtpbaseplotter.cpp \
           $$PWD/jkqtpdatastorage.cpp \
           $$PWD/jkqtpelements.cpp \
           $$PWD/jkqtmathtext.cpp \
           $$PWD/jkqtpbaseelements.cpp \
           $$PWD/jkqtplotter.cpp \
           $$PWD/jkqtptools.cpp \
           $$PWD/jkqttools.cpp  \
           $$PWD/jkqtpimageelements.cpp  \
           $$PWD/jkqtpimagetools.cpp  \
           $$PWD/jkqtpparsedfunctionelements.cpp  \
           $$PWD/jkqtpoverlayelements.cpp  \
           $$PWD/jkqtpgeoelements.cpp  \
           $$PWD/jkqtpmathparser.cpp \
           $$PWD/jkqtpplotsmodel.cpp


RESOURCES += $$PWD/jkqtpbaseplotter.qrc

INCLUDEPATH += $$PWD 

QT += core gui svg
win32:LIBS += -lgdi32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


# for support of palettes stored as XML, use:
QT += xml
