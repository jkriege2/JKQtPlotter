TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib datastore_iterators

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

datastore_iterators.file=$$PWD/datastore_iterators.pro
datastore_iterators.depends = jkqtplotterlib
