
isEmpty(JKQTP_FASTPLOTTER_PRI_INCLUDED) {
    JKQTP_FASTPLOTTER_PRI_INCLUDED = 1

    INCLUDEPATH += $PWD


    HEADERS += $$PWD/jkqtfastplotter/jkqtfastplotter.h


    SOURCES += $$PWD/jkqtfastplotter/jkqtfastplotter.cpp


    RESOURCES += $$PWD/jkqtplotter/resources/jkqtpbaseplotter.qrc

    QT += opengl
    greaterThan(QT_MAJOR_VERSION, 5) {
        QT += openglwidgets
    }

}
