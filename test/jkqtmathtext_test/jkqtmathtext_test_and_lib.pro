TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtmathtext_test

jkqtmathtext_test.file=$$PWD/jkqtmathtext_test.pro
jkqtmathtext_test.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
