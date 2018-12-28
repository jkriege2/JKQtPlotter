TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest.file=$$PWD/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib
