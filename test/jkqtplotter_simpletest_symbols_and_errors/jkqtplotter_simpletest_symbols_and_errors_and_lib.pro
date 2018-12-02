TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_symbols_and_errors

jkqtplotter_simpletest_symbols_and_errors.file=$$PWD/jkqtplotter_simpletest_symbols_and_errors.pro
jkqtplotter_simpletest_symbols_and_errors.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
