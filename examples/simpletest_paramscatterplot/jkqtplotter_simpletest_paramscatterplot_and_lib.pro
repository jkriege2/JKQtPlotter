TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_paramscatterplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_paramscatterplot.file=$$PWD/jkqtplotter_simpletest_paramscatterplot.pro
jkqtplotter_simpletest_paramscatterplot.depends = jkqtplotterlib
