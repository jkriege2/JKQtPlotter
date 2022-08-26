TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib scatter

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

scatter.file=$$PWD/scatter.pro
scatter.depends = jkqtplotterlib
