HEADERS += $$PWD/jkqtmathtext.h \
           $$PWD/jkqtptools.h \
           $$PWD/jkqtp_imexport.h


SOURCES += $$PWD/jkqtmathtext.cpp \
           $$PWD/jkqtptools.cpp


INCLUDEPATH += $$PWD 

QT += core gui
win32:LIBS += -lgdi32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
