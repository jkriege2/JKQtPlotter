TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_simpletest_geometric

jkqtplotterlib.file = ../../staticlib/jkqtplotterlib.pro

jkqtplotter_simpletest_geometric.file=$$PWD/jkqtplotter_simpletest_geometric.pro
jkqtplotter_simpletest_geometric.depends = jkqtplotterlib
