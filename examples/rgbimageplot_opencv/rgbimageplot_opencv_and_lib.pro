TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib rgbimageplot_opencv

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

rgbimageplot_opencv.file=$$PWD/rgbimageplot_opencv.pro
rgbimageplot_opencv.depends = jkqtplotterlib
