TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib paramscatterplot_image

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

paramscatterplot_image.file=$$PWD/paramscatterplot_image.pro
paramscatterplot_image.depends = jkqtplotterlib
