TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib imageplot_modifier

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

imageplot_modifier.file=$$PWD/imageplot_modifier.pro
imageplot_modifier.depends = jkqtplotterlib
