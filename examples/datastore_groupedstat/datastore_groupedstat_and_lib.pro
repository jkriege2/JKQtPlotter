TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib datastore_groupedstat

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

datastore_groupedstat.file=$$PWD/datastore_groupedstat.pro
datastore_groupedstat.depends = jkqtplotterlib
