TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtmathtext_simpletest

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtmathtext_simpletest.file=$$PWD/jkqtmathtext_simpletest.pro
jkqtmathtext_simpletest.depends = jkqtplotterlib
