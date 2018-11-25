HEADERS += $$PWD/jkqtmathtext/jkqtmathtext.h \
           $$PWD/jkqtplottertools/jkqtptools.h \
           $$PWD/jkqtplottertools/jkqtp_imexport.h


SOURCES += $$PWD/jkqtmathtext/jkqtmathtext.cpp \
           $$PWD/jkqtplottertools/jkqtptools.cpp


INCLUDEPATH += $$PWD 

QT += core gui
#win32:LIBS += -lgdi32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
