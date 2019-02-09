TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_rgbimageplot_opencv

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_rgbimageplot_opencv.file=$$PWD/jkqtplotter_simpletest_rgbimageplot_opencv.pro
jkqtplotter_simpletest_rgbimageplot_opencv.depends = jkqtplotterlib
