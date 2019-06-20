TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib rgbimageplot_qt

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

rgbimageplot_qt.file=$$PWD/rgbimageplot_qt.pro
rgbimageplot_qt.depends = jkqtplotterlib
