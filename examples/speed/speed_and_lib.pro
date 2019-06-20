TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib speed

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

speed.file=$$PWD/speed.pro
speed.depends = jkqtplotterlib
