TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_datastore_groupedstat

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_datastore_groupedstat.file=$$PWD/jkqtplotter_simpletest_datastore_groupedstat.pro
jkqtplotter_simpletest_datastore_groupedstat.depends = jkqtplotterlib
