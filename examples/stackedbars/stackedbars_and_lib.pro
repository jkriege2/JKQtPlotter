TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib stackedbars

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

stackedbars.file=$$PWD/stackedbars.pro
stackedbars.depends = jkqtplotterlib
