TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib test_user_interaction

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

test_user_interaction.file=$$PWD/test_user_interaction.pro
test_user_interaction.depends = jkqtplotterlib
