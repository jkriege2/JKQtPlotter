TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_datastore_statistics_2d

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_datastore_statistics_2d.file=$$PWD/jkqtplotter_simpletest_datastore_statistics_2d.pro
jkqtplotter_simpletest_datastore_statistics_2d.depends = jkqtplotterlib
