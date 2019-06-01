TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_datastore_regression

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_datastore_regression.file=$$PWD/jkqtplotter_simpletest_datastore_regression.pro
jkqtplotter_simpletest_datastore_regression.depends = jkqtplotterlib
