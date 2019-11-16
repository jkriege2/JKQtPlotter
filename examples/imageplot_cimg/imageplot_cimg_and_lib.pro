TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib imageplot_cimg

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

imageplot_cimg.file=$$PWD/imageplot_cimg.pro
imageplot_cimg.depends = jkqtplotterlib
