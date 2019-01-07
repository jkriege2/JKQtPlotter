TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_stepplots

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_stepplots.file=$$PWD/jkqtplotter_simpletest_stepplots.pro
jkqtplotter_simpletest_stepplots.depends = jkqtplotterlib
