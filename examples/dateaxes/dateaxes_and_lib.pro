TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib dateaxes

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

dateaxes.file=$$PWD/dateaxes.pro
dateaxes.depends = jkqtplotterlib
