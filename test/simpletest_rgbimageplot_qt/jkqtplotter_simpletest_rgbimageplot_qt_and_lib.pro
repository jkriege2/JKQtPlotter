TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_rgbimageplot_qt

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest_rgbimageplot_qt.file=$$PWD/jkqtplotter_simpletest_rgbimageplot_qt.pro
jkqtplotter_simpletest_rgbimageplot_qt.depends = jkqtplotterlib
