TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib rgbimageplot_cimg

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

rgbimageplot_cimg.file=$$PWD/rgbimageplot_cimg.pro
rgbimageplot_cimg.depends = jkqtplotterlib
