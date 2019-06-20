TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib simpletest

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

simpletest.file=$$PWD/simpletest.pro
simpletest.depends = jkqtplotterlib
