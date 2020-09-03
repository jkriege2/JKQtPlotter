TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib geo_simple

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

geo_simple.file=$$PWD/geo_simple.pro
geo_simple.depends = jkqtplotterlib
