TEMPLATE = subdirs

SUBDIRS +=  jkqtmathtextlib jkqtmathtext_render

jkqtmathtextlib.file = ../../qmake/staticlib/jkqtmathtextlib/jkqtmathtextlib.pro

jkqtmathtext_render.file=$$PWD/jkqtmathtext_render.pro
jkqtmathtext_render.depends = jkqtmathtextlib
