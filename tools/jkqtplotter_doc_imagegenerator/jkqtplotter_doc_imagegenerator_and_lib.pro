TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib jkqtplotter_doc_imagegenerator

jkqtmathtextlib.file = ../../qmake/staticlib/jkqtmathtextlib/jkqtmathtextlib.pro

jkqtplotter_doc_imagegenerator.file=$$PWD/jkqtplotter_doc_imagegenerator.pro
jkqtplotter_doc_imagegenerator.depends = jkqtplotterlib
