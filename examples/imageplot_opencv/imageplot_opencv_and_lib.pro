TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib imageplot_opencv

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

imageplot_opencv.file=$$PWD/imageplot_opencv.pro
imageplot_opencv.depends = jkqtplotterlib
