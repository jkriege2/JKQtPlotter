TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib violinplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

violinplot.file=$$PWD/violinplot.pro
violinplot.depends = jkqtplotterlib
