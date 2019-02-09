TEMPLATE = subdirs

SUBDIRS +=  jkqtmathtextlib jkqtphighrestimerlib jkqtmathtext_test

jkqtmathtextlib.file = ../../staticlib/jkqtmathtextlib/jkqtmathtextlib.pro
jkqtphighrestimerlib.file = ../../staticlib/jkqtphighrestimerlib/jkqtphighrestimerlib.pro

jkqtmathtext_test.file=$$PWD/jkqtmathtext_test.pro
jkqtmathtext_test.depends = jkqtmathtextlib jkqtphighrestimerlib
