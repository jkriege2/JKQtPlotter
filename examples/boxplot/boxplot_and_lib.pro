TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib boxplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

boxplot.file=$$PWD/boxplot.pro
boxplot.depends = jkqtplotterlib
