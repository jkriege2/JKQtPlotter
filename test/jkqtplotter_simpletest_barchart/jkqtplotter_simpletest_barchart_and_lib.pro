TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_barchart

jkqtplotter_simpletest_barchart.file=$$PWD/jkqtplotter_simpletest_barchart.pro
jkqtplotter_simpletest_barchart.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
