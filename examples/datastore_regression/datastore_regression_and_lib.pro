TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib datastore_regression

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

datastore_regression.file=$$PWD/datastore_regression.pro
datastore_regression.depends = jkqtplotterlib
