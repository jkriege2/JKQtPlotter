TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_boxplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_boxplot.file=$$PWD/jkqtplotter_simpletest_boxplot.pro
jkqtplotter_simpletest_boxplot.depends = jkqtplotterlib
