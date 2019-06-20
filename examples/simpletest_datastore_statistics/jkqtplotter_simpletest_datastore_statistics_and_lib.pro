TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_datastore_statistics

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_datastore_statistics.file=$$PWD/jkqtplotter_simpletest_datastore_statistics.pro
jkqtplotter_simpletest_datastore_statistics.depends = jkqtplotterlib
