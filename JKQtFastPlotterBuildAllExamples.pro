TEMPLATE = subdirs

SUBDIRS +=  jkqtfastplotterlib \
            jkqtfastplotter_test

jkqtfastplotterlib.file = lib/jkqtfastplotterlib.pro

jkqtfastplotter_test.subdir = test/jkqtfastplotter_test
jkqtfastplotter_test.depends = jkqtfastplotterlib
