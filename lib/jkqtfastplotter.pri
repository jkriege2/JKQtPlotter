

QMAKE_CXXFLAGS += -fexceptions


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

