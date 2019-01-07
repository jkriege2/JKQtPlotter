TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_imageplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_imageplot.file=$$PWD/jkqtplotter_simpletest_imageplot.pro
jkqtplotter_simpletest_imageplot.depends = jkqtplotterlib
