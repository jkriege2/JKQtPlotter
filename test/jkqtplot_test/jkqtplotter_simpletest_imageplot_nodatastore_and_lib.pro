TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplot_test

jkqtplot_test.file=$$PWD/jkqtplot_test.pro
jkqtplot_test.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
