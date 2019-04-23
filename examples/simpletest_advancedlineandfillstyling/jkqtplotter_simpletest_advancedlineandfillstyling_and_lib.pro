TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_advancedlineandfillstyling

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtplotter_simpletest_advancedlineandfillstyling.file=$$PWD/jkqtplotter_simpletest_advancedlineandfillstyling.pro
jkqtplotter_simpletest_advancedlineandfillstyling.depends = jkqtplotterlib
