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

jkqtplotter_simpletest.subdir = test/jkqtplotter_simpletest
jkqtplotter_simpletest.depends = jkqtplotterlib

jkqtplotter_simpletest_barchart.subdir = test/jkqtplotter_simpletest_barchart
jkqtplotter_simpletest_barchart.depends = jkqtplotterlib

jkqtplotter_simpletest_dateaxes.subdir = test/jkqtplotter_simpletest_dateaxes
jkqtplotter_simpletest_dateaxes.depends = jkqtplotterlib

jkqtplotter_simpletest_errorbarstyles.subdir = test/jkqtplotter_simpletest_errorbarstyles
jkqtplotter_simpletest_errorbarstyles.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot.subdir = test/jkqtplotter_simpletest_imageplot
jkqtplotter_simpletest_imageplot.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot_modifier.subdir = test/jkqtplotter_simpletest_imageplot_modifier
jkqtplotter_simpletest_imageplot_modifier.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot_nodatastore.subdir = test/jkqtplotter_simpletest_imageplot_nodatastore
jkqtplotter_simpletest_imageplot_nodatastore.depends = jkqtplotterlib

jkqtplotter_simpletest_imageplot_opencv.subdir = test/jkqtplotter_simpletest_imageplot_opencv

jkqtplotter_simpletest_logaxes.subdir = test/jkqtplotter_simpletest_logaxes
jkqtplotter_simpletest_logaxes.depends = jkqtplotterlib

jkqtplotter_simpletest_rgbimageplot_opencv.subdir = test/jkqtplotter_simpletest_rgbimageplot_opencv

jkqtplotter_simpletest_stackedbars.subdir = test/jkqtplotter_simpletest_stackedbars
jkqtplotter_simpletest_stackedbars.depends = jkqtplotterlib

jkqtplotter_simpletest_symbols_and_errors.subdir = test/jkqtplotter_simpletest_symbols_and_errors
jkqtplotter_simpletest_symbols_and_errors.depends = jkqtplotterlib

jkqtplotter_simpletest_symbols_and_styles.subdir = test/jkqtplotter_simpletest_symbols_and_styles
jkqtplotter_simpletest_symbols_and_styles.depends = jkqtplotterlib

jkqtplotter_simpletest_speed.subdir = test/jkqtplotter_simpletest_speed
jkqtplotter_simpletest_speed.depends = jkqtplotterlib
