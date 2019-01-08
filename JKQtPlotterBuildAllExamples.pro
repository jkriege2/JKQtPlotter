TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
			jkqtplotterlib_sharedlib \
            jkqtmathtext_simpletest \
            jkqtplot_test \
            jkqtplotter_simpletest \
			test_multiplot \
			jkqtfastplotter_test


jkqtplotterlib.file = staticlib/jkqtplotterlib.pro

jkqtplotterlib_sharedlib.file = sharedlib/jkqtplotterlib_sharedlib.pro

jkqtmathtext_simpletest.subdir = examples/jkqtmathtext_simpletest
jkqtmathtext_simpletest.depends = jkqtplotterlib

jkqtmathtext_test.subdir = examples/jkqtmathtext_test
jkqtmathtext_test.depends = jkqtplotterlib

jkqtplot_test.subdir = examples/jkqtplot_test
jkqtplot_test.depends = jkqtplotterlib

jkqtplotter_simpletest.file = examples/simpletest/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib

test_multiplot.file = examples/test_multiplot/test_multiplot.pro
test_multiplot.depends = jkqtplotterlib

jkqtfastplotter_test.file = $$PWD/examples/jkqtfastplotter_test/jkqtfastplotter_test.pro
jkqtfastplotter_test.depends = jkqtplotterlib

defineTest(addSimpleTest) {
    test_name = $$1
    SUBDIRS += jkqtplotter_simpletest_$${test_name}

    jkqtplotter_simpletest_$${test_name}.file = examples/simpletest_$${test_name}/jkqtplotter_simpletest_$${test_name}.pro
    jkqtplotter_simpletest_$${test_name}.depends = jkqtplotterlib

    export (jkqtplotter_simpletest_$${test_name}.file)
    export (jkqtplotter_simpletest_$${test_name}.depends)

    export (SUBDIRS)
}

addSimpleTest(barchart)
addSimpleTest(dateaxes)
addSimpleTest(errorbarstyles)
addSimpleTest(imageplot)
addSimpleTest(imageplot_modifier)
addSimpleTest(logaxes)
addSimpleTest(stackedbars)
addSimpleTest(symbols_and_errors)
addSimpleTest(symbols_and_styles)
addSimpleTest(filledgraphs)
addSimpleTest(speed)
addSimpleTest(rgbimageplot_qt)
addSimpleTest(impulsesplot)
addSimpleTest(paramscatterplot)
addSimpleTest(paramscatterplot_image)
addSimpleTest(parametriccurve)
addSimpleTest(parsedfunctionplot)
addSimpleTest(functionplot)
addSimpleTest(geometric)
addSimpleTest(ui)
#addSimpleTest(imageplot_nodatastore)
#addSimpleTest(rgbimageplot_opencv)
#addSimpleTest(imageplot_opencv)
