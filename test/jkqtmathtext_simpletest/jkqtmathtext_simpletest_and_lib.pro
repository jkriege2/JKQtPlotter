TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtmathtext_simpletest

jkqtmathtext_simpletest.file=$$PWD/jkqtmathtext_simpletest.pro
jkqtmathtext_simpletest.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
