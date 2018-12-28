TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_logaxes

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest_logaxes.file=$$PWD/jkqtplotter_simpletest_logaxes.pro
jkqtplotter_simpletest_logaxes.depends = jkqtplotterlib
