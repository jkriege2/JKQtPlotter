TEMPLATE = subdirs

SUBDIRS +=  jkqtfastplotterlib jkqtfastplotter_test

jkqtfastplotterlib.file = ../../lib/jkqtfastplotterlib.pro

jkqtfastplotter_test.file=$$PWD/jkqtfastplotter_test.pro
jkqtfastplotter_test.depends = jkqtfastplotterlib
