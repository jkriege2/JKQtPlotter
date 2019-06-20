TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib datastore

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

datastore.file=$$PWD/datastore.pro
datastore.depends = jkqtplotterlib
