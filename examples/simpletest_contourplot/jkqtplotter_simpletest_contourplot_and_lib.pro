TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_contourplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_contourplot.file=$$PWD/jkqtplotter_simpletest_contourplot.pro
jkqtplotter_simpletest_contourplot.depends = jkqtplotterlib
