TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib logaxes

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

logaxes.file=$$PWD/logaxes.pro
logaxes.depends = jkqtplotterlib
