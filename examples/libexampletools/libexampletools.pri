
isEmpty(JKQTP_LIBEXAMPLETOOLS_PRI_INCLUDED) {
    JKQTP_LIBEXAMPLETOOLS_PRI_INCLUDED = 1

    INCLUDEPATH += $PWD


    HEADERS += $$PWD/jkqtpexampleapplication.h \
               $$PWD/jkqtpappsettingcontroller.h


    SOURCES += $$PWD/jkqtpexampleapplication.cpp \
               $$PWD/jkqtpappsettingcontroller.cpp


}
