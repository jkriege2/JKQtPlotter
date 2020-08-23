TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib geo_arrows

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

geo_arrows.file=$$PWD/geo_arrows.pro
geo_arrows.depends = jkqtplotterlib
