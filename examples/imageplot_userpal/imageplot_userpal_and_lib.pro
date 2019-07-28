TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib imageplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

imageplot.file=$$PWD/imageplot.pro
imageplot.depends = jkqtplotterlib
