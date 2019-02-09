TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_parametriccurve

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_parametriccurve.file=$$PWD/jkqtplotter_simpletest_parametriccurve.pro
jkqtplotter_simpletest_parametriccurve.depends = jkqtplotterlib
