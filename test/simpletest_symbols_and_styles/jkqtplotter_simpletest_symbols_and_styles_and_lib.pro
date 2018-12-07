TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_symbols_and_styles

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro

jkqtplotter_simpletest_symbols_and_styles.file=$$PWD/jkqtplotter_simpletest_symbols_and_styles.pro
jkqtplotter_simpletest_symbols_and_styles.depends = jkqtplotterlib
