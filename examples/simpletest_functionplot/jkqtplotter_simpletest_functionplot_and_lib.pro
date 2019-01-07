TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_functionplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_functionplot.file=$$PWD/jkqtplotter_simpletest_functionplot.pro
jkqtplotter_simpletest_functionplot.depends = jkqtplotterlib
