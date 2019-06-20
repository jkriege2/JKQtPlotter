TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib contourplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

contourplot.file=$$PWD/contourplot.pro
contourplot.depends = jkqtplotterlib
