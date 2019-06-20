TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib imageplot_nodatastore

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

imageplot_nodatastore.file=$$PWD/imageplot_nodatastore.pro
imageplot_nodatastore.depends = jkqtplotterlib
