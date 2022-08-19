
isEmpty(JKQTP_MATHTEXT_PRI_INCLUDED) {
    JKQTP_MATHTEXT_PRI_INCLUDED = 1


    INCLUDEPATH += $PWD

    HEADERS += $$PWD/jkqtmathtext/jkqtmathtext.h \
               $$PWD/jkqtmathtext/jkqtmathtextlabel.h \
               $$PWD/jkqtmathtext/jkqtmathtexttools.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtexttextnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextbracenode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextdecoratednode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextfracnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextinstructionnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextlistnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextverticallistnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtexthorizontallistnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextmatrixnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextsqrtnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextsubsupernode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextsymbolnode.h  \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextwhitespacenode.h  \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextnodetools.h  \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextnoopnode.h \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextverbatimnode.h  \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextboxinstructionnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextmodifyenvironmentnode.cpp


    SOURCES += $$PWD/jkqtmathtext/jkqtmathtext.cpp \
               $$PWD/jkqtmathtext/jkqtmathtextlabel.cpp \
               $$PWD/jkqtmathtext/jkqtmathtexttools.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtexttextnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextbracenode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextdecoratednode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextfracnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextinstructionnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextverticallistnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtexthorizontallistnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextmatrixnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextsqrtnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextsubsupernode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextsymbolnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextwhitespacenode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextnodetools.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextnoopnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextverbatimnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextboxinstructionnode.cpp \
               $$PWD/jkqtmathtext/nodes/jkqtmathtextmodifyenvironmentnode.cpp

    include($$PWD/jkqtmathtext/resources/xits.pri)
    DEFINES += AUTOLOAD_XITS_FONTS
    DEFINES += USE_XITS_FONTS

    #win32:LIBS += -lgdi32
}
