TEMPLATE = subdirs

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

test_styling.file=$$PWD/test_styling.pro
test_styling.depends = jkqtplotterlib

SUBDIRS +=  jkqtplotterlib test_styling
