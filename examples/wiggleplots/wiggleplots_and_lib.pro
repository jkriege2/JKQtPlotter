TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib wiggleplots

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

wiggleplots.file=$$PWD/wiggleplots.pro
wiggleplots.depends = jkqtplotterlib
