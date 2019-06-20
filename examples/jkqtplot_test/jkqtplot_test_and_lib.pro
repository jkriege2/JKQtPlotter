TEMPLATE = subdirs

SUBDIRS +=  jkqtplot_test jkqtplotterlib

jkqtplot_test.file=$$PWD/jkqtplot_test.pro
jkqtplot_test.depends = jkqtplotterlib

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro
