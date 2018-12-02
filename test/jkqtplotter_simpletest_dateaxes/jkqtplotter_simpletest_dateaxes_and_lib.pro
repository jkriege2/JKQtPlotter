TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_dateaxes

jkqtplotter_simpletest_dateaxes.file=$$PWD/jkqtplotter_simpletest_dateaxes.pro
jkqtplotter_simpletest_dateaxes.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
