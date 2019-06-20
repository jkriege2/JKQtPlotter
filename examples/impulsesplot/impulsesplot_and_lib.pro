TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib impulsesplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

impulsesplot.file=$$PWD/impulsesplot.pro
impulsesplot.depends = jkqtplotterlib
