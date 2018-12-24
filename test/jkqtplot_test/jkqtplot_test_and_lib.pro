TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplot_test

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplot_test.file=$$PWD/jkqtplot_test.pro
jkqtplot_test.depends = jkqtplotterlib
