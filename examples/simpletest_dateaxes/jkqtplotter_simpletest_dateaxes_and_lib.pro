TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_dateaxes

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_dateaxes.file=$$PWD/jkqtplotter_simpletest_dateaxes.pro
jkqtplotter_simpletest_dateaxes.depends = jkqtplotterlib
