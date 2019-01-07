TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtmathtext_test

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtmathtext_test.file=$$PWD/jkqtmathtext_test.pro
jkqtmathtext_test.depends = jkqtplotterlib
