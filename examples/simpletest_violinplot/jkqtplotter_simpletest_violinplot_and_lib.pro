TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_violinplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_violinplot.file=$$PWD/jkqtplotter_simpletest_violinplot.pro
jkqtplotter_simpletest_violinplot.depends = jkqtplotterlib
