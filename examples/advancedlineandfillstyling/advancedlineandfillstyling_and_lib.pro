TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib advancedlineandfillstyling

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

advancedlineandfillstyling.file=$$PWD/advancedlineandfillstyling.pro
advancedlineandfillstyling.depends = jkqtplotterlib
