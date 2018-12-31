
include($$PWD/common.pri)


HEADERS += $$PWD/jkqtfastplotter/jkqtfastplotter.h \
           $$PWD/jkqtplottertools/jkqtphighrestimer.h


SOURCES += $$PWD/jkqtfastplotter/jkqtfastplotter.cpp \
           $$PWD/jkqtplottertools/jkqtphighrestimer.cpp


RESOURCES += $$PWD/jkqtplotterressources/jkqtpbaseplotter.qrc

QT += opengl
