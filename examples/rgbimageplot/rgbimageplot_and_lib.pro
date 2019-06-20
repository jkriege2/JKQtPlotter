TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib rgbimageplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

rgbimageplot.file=$$PWD/rgbimageplot.pro
rgbimageplot.depends = jkqtplotterlib
