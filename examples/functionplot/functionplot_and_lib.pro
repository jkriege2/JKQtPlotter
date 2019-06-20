TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib functionplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

functionplot.file=$$PWD/functionplot.pro
functionplot.depends = jkqtplotterlib
