TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
            jkqtmathtext_simpletest \
            jkqtplotter_simpletest_symbols_and_styles \
            jkqtplotter_simpletest_symbols_and_errors \
            jkqtplotter_simpletest_stackedbars \
            jkqtplotter_simpletest_logaxes \
            jkqtplotter_simpletest_imageplot_modifier \
            jkqtplotter_simpletest_imageplot \
            jkqtplotter_simpletest_errorbarstyles \
            jkqtplotter_simpletest_dateaxes \
            jkqtplotter_simpletest_barchart \
            jkqtplotter_simpletest \
            jkqtplotter_simpletest_speed \
            jkqtplotter_simpletest_rgbimageplot_qt \
            jkqtplot_test
            #jkqtplotter_simpletest_imageplot_nodatastore \
            #jkqtplotter_simpletest_rgbimageplot_opencv \
            #jkqtplotter_simpletest_imageplot_opencv \





jkqtplotterlib.file = lib/jkqtplotterlib.pro

jkqtmathtext_simpletest.subdir = test/jkqtmathtext_simpletest
jkqtmathtext_simpletest.depends = jkqtplotterlib

jkqtmathtext_test.subdir = test/jkqtmathtext_test
jkqtmathtext_test.depends = jkqtplotterlib

jkqtplot_test.subdir = test/jkqtplot_test
jkqtplot_test.depends = jkqtplotterlib

jkqtplotter_simpletest.subdir = test/simpletest
jkqtplotter_simpletest.depends = jkqtplotterlib

jkqtplotter_simpletest_barchart.subdir = test/simpletest_barchart
jkqtplotter_simpletest_barchart.depends = jkqtplotterlib

jkqtplotter_simpletest_dateaxes.subdir = test/simpletest_dateaxes
jkqtplotter_simpletest_dateaxes.depends = jkqtplotterlib

jkqtplotter_simpletest_errorbarstyles.subdir = test/simpletest_errorbarstyles
jkqtplotter_simpletest_errorbarstyles.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot.subdir = test/simpletest_imageplot
jkqtplotter_simpletest_imageplot.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot_modifier.subdir = test/simpletest_imageplot_modifier
jkqtplotter_simpletest_imageplot_modifier.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot_nodatastore.subdir = test/simpletest_imageplot_nodatastore
jkqtplotter_simpletest_imageplot_nodatastore.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot_opencv.subdir = test/simpletest_imageplot_opencv

jkqtplotter_simpletest_logaxes.subdir = test/simpletest_logaxes
jkqtplotter_simpletest_logaxes.depends = jkqtplotterlib

jkqtplotter_simpletest_rgbimageplot_opencv.subdir = test/simpletest_rgbimageplot_opencv

jkqtplotter_simpletest_stackedbars.subdir = test/simpletest_stackedbars
jkqtplotter_simpletest_stackedbars.depends = jkqtplotterlib

jkqtplotter_simpletest_symbols_and_errors.subdir = test/simpletest_symbols_and_errors
jkqtplotter_simpletest_symbols_and_errors.depends = jkqtplotterlib

jkqtplotter_simpletest_symbols_and_styles.subdir = test/simpletest_symbols_and_styles
jkqtplotter_simpletest_symbols_and_styles.depends = jkqtplotterlib

jkqtplotter_simpletest_speed.subdir = test/simpletest_speed
jkqtplotter_simpletest_speed.depends = jkqtplotterlib

jkqtplotter_simpletest_rgbimageplot_qt.subdir = test/simpletest_rgbimageplot_qt
jkqtplotter_simpletest_rgbimageplot_qt.depends = jkqtplotterlib
