TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib symbols_and_styles

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

symbols_and_styles.file=$$PWD/symbols_and_styles.pro
symbols_and_styles.depends = jkqtplotterlib
