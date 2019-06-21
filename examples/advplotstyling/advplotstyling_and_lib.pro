TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib advplotstyling

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

advplotstyling.file=$$PWD/advplotstyling.pro
advplotstyling.depends = jkqtplotterlib
