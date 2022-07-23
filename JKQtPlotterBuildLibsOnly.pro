TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
            jkqtplotterlib_sharedlib \
            jkqtmathtextlib \
            jkqtmathtextlib_sharedlib \
            jkqtcommonlib \
            jkqtcommonlib_sharedlib \
            jkqtfastplotterlib \
            jkqtfastplotterlib_sharedlib


jkqtplotterlib.file = qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro
jkqtplotterlib_sharedlib.file = qmake/sharedlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtmathtextlib.file = qmake/staticlib/jkqtmathtextlib/jkqtmathtextlib.pro
jkqtmathtextlib_sharedlib.file = qmake/sharedlib/jkqtmathtextlib/jkqtmathtextlib.pro

jkqtcommonlib.file = qmake/staticlib/jkqtcommonlib/jkqtcommonlib.pro
jkqtcommonlib_sharedlib.file = qmake/sharedlib/jkqtcommonlib/jkqtcommonlib.pro

jkqtfastplotterlib.file = qmake/staticlib/jkqtfastplotterlib/jkqtfastplotterlib.pro
jkqtfastplotterlib_sharedlib.file = qmake/sharedlib/jkqtfastplotterlib/jkqtfastplotterlib.pro
