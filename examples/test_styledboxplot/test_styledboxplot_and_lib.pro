TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib test_styledboxplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

test_styledboxplot.file=$$PWD/test_styledboxplot.pro
test_styledboxplot.depends = jkqtplotterlib
