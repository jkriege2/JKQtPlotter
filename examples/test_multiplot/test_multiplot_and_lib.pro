TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib test_multiplot

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

test_multiplot.file=$$PWD/test_multiplot.pro
test_multiplot.depends = jkqtplotterlib
