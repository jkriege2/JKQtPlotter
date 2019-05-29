
isEmpty(JKQTP_MATHTEXT_PRI_INCLUDED) {
    JKQTP_MATHTEXT_PRI_INCLUDED = 1


    INCLUDEPATH += $PWD

    HEADERS += $$PWD/jkqtmathtext/jkqtmathtext.h

    SOURCES += $$PWD/jkqtmathtext/jkqtmathtext.cpp

    include($$PWD/jkqtplotterressources/math_fonts/xits.pri)
    DEFINES += AUTOLOAD_XITS_FONTS
    DEFINES += USE_XITS_FONTS

    #win32:LIBS += -lgdi32
}
