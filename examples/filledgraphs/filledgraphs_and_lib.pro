TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib filledgraphs

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

filledgraphs.file=$$PWD/filledgraphs.pro
filledgraphs.depends = jkqtplotterlib
