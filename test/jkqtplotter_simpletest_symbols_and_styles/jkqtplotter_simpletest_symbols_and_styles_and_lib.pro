TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_symbols_and_styles

jkqtplotter_simpletest_symbols_and_styles.file=$$PWD/jkqtplotter_simpletest_symbols_and_styles.pro
jkqtplotter_simpletest_symbols_and_styles.depends = jkqtplotterlib

jkqtplotterlib.file = ../../lib/jkqtplotterlib.pro
