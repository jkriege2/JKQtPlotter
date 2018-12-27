TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplottest

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplottest.file=$$PWD/jkqtplot_test.pro
jkqtplottest.depends = jkqtplotterlib
