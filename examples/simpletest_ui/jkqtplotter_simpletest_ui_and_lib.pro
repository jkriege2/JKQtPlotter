TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_ui

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_ui.file=$$PWD/jkqtplotter_simpletest_ui.pro
jkqtplotter_simpletest_ui.depends = jkqtplotterlib
