TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib barchart

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

barchart.file=$$PWD/barchart.pro
barchart.depends = jkqtplotterlib
