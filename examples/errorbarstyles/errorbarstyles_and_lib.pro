TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib errorbarstyles

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

errorbarstyles.file=$$PWD/errorbarstyles.pro
errorbarstyles.depends = jkqtplotterlib
