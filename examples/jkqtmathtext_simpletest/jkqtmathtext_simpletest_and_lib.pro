TEMPLATE = subdirs

SUBDIRS +=  jkqtmathtextlib jkqtmathtext_simpletest

jkqtmathtextlib.file = ../../qmake/staticlib/jkqtmathtextlib/jkqtmathtextlib.pro

jkqtmathtext_simpletest.file=$$PWD/jkqtmathtext_simpletest.pro
jkqtmathtext_simpletest.depends = jkqtmathtextlib
