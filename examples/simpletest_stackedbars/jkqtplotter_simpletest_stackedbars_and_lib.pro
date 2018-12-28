TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_stackedbars

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest_stackedbars.file=$$PWD/jkqtplotter_simpletest_stackedbars.pro
jkqtplotter_simpletest_stackedbars.depends = jkqtplotterlib
