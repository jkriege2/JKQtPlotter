TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_datastore_iterators

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_datastore_iterators.file=$$PWD/jkqtplotter_simpletest_datastore_iterators.pro
jkqtplotter_simpletest_datastore_iterators.depends = jkqtplotterlib
