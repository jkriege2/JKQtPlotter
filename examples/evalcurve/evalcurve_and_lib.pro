TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib evalcurve

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

evalcurve.file=$$PWD/evalcurve.pro
evalcurve.depends = jkqtplotterlib
