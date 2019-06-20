TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib symbols_and_errors

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

symbols_and_errors.file=$$PWD/symbols_and_errors.pro
symbols_and_errors.depends = jkqtplotterlib
