TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib stepplots

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

stepplots.file=$$PWD/stepplots.pro
stepplots.depends = jkqtplotterlib
