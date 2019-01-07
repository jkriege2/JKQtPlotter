TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_symbols_and_errors

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_symbols_and_errors.file=$$PWD/jkqtplotter_simpletest_symbols_and_errors.pro
jkqtplotter_simpletest_symbols_and_errors.depends = jkqtplotterlib
