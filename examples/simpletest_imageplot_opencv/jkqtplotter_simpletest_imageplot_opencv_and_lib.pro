TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_imageplot_opencv

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_imageplot_opencv.file=$$PWD/jkqtplotter_simpletest_imageplot_opencv.pro
jkqtplotter_simpletest_imageplot_opencv.depends = jkqtplotterlib
