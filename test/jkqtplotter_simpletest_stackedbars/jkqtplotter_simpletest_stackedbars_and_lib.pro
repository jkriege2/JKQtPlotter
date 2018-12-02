TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_stackedbars

jkqtplotter_simpletest_stackedbars.file=$$PWD/jkqtplotter_simpletest_stackedbars.pro
jkqtplotter_simpletest_stackedbars.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
