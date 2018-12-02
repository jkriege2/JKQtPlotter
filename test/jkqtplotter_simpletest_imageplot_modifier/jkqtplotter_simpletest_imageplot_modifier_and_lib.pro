TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_imageplot_modifier

jkqtplotter_simpletest_imageplot_modifier.file=$$PWD/jkqtplotter_simpletest_imageplot_modifier.pro
jkqtplotter_simpletest_imageplot_modifier.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
