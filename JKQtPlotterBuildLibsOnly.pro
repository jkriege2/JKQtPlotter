TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
            jkqtplotterlib_sharedlib \
            jkqtmathtextlib \
            jkqtmathtextlib_sharedlib \
            jkqtpcommonlib \
            jkqtpcommonlib_sharedlib \
            jkqtfastplotterlib \
            jkqtfastplotterlib_sharedlib


jkqtplotterlib.file = qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro
jkqtplotterlib_sharedlib.file = qmake/sharedlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtmathtextlib.file = qmake/staticlib/jkqtmathtextlib/jkqtmathtextlib.pro
jkqtmathtextlib_sharedlib.file = qmake/sharedlib/jkqtmathtextlib/jkqtmathtextlib.pro

jkqtpcommonlib.file = qmake/staticlib/jkqtpcommonlib/jkqtpcommonlib.pro
jkqtpcommonlib_sharedlib.file = qmake/sharedlib/jkqtpcommonlib/jkqtpcommonlib.pro

jkqtfastplotterlib.file = qmake/staticlib/jkqtfastplotterlib/jkqtfastplotterlib.pro
jkqtfastplotterlib_sharedlib.file = qmake/sharedlib/jkqtfastplotterlib/jkqtfastplotterlib.pro
