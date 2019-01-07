TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_logaxes

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_logaxes.file=$$PWD/jkqtplotter_simpletest_logaxes.pro
jkqtplotter_simpletest_logaxes.depends = jkqtplotterlib
