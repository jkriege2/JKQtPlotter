TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_parsedfunctionplot

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_parsedfunctionplot.file=$$PWD/jkqtplotter_simpletest_parsedfunctionplot.pro
jkqtplotter_simpletest_parsedfunctionplot.depends = jkqtplotterlib
