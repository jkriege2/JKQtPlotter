TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib geometric

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

geometric.file=$$PWD/geometric.pro
geometric.depends = jkqtplotterlib
