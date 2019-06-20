TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib datastore_statistics

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

datastore_statistics.file=$$PWD/datastore_statistics.pro
datastore_statistics.depends = jkqtplotterlib
