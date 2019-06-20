
isEmpty(JKQTP_FASTPLOTTER_PRI_INCLUDED) {
    JKQTP_FASTPLOTTER_PRI_INCLUDED = 1

    INCLUDEPATH += $PWD


    HEADERS += $$PWD/jkqtfastplotter/jkqtfastplotter.h


    SOURCES += $$PWD/jkqtfastplotter/jkqtfastplotter.cpp


    RESOURCES += $$PWD/jkqtplotterressources/jkqtpbaseplotter.qrc

    QT += opengl

}
