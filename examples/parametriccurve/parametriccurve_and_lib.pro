TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib parametriccurve

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

parametriccurve.file=$$PWD/parametriccurve.pro
parametriccurve.depends = jkqtplotterlib
