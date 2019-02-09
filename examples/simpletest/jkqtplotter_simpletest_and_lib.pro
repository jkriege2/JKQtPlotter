TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest.file=$$PWD/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib
