TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtfastplotter_test

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtfastplotter_test.file=$$PWD/jkqtfastplotter_test.pro
jkqtfastplotter_test.depends = jkqtplotterlib
