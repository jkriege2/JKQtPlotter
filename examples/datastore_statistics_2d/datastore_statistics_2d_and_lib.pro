TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib datastore_statistics_2d

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

datastore_statistics_2d.file=$$PWD/datastore_statistics_2d.pro
datastore_statistics_2d.depends = jkqtplotterlib
