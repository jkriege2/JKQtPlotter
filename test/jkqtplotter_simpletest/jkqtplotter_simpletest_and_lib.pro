TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest

jkqtplotter_simpletest.file=$$PWD/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
