TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtmathtext_test

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtmathtext_test.file=$$PWD/jkqtmathtext_test.pro
jkqtmathtext_test.depends = jkqtplotterlib
