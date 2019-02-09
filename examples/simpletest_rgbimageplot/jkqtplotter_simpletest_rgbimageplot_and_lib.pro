TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_rgbimageplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_rgbimageplot.file=$$PWD/jkqtplotter_simpletest_rgbimageplot.pro
jkqtplotter_simpletest_rgbimageplot.depends = jkqtplotterlib
