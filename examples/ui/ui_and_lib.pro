TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib ui

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

ui.file=$$PWD/ui.pro
ui.depends = jkqtplotterlib
