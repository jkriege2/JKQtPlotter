TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_speed

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest_speed.file=$$PWD/jkqtplotter_simpletest_speed.pro
jkqtplotter_simpletest_speed.depends = jkqtplotterlib
