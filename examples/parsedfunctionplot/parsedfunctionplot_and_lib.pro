TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib parsedfunctionplot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

parsedfunctionplot.file=$$PWD/parsedfunctionplot.pro
parsedfunctionplot.depends = jkqtplotterlib
