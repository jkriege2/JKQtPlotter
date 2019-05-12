TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_datastore

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_datastore.file=$$PWD/jkqtplotter_simpletest_datastore.pro
jkqtplotter_simpletest_datastore.depends = jkqtplotterlib
