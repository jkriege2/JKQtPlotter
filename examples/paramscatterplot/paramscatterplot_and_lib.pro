TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib paramscatterplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

paramscatterplot.file=$$PWD/paramscatterplot.pro
paramscatterplot.depends = jkqtplotterlib
