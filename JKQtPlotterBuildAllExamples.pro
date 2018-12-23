TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
            jkqtmathtext_simpletest \
            jkqtplot_test \
            jkqtplotter_simpletest \
			test_multiplot


jkqtplotterlib.file = lib/jkqtplotterlib.pro

jkqtmathtext_simpletest.subdir = test/jkqtmathtext_simpletest
jkqtmathtext_simpletest.depends = jkqtplotterlib

jkqtmathtext_test.subdir = test/jkqtmathtext_test
jkqtmathtext_test.depends = jkqtplotterlib

jkqtplot_test.subdir = test/jkqtplot_test
jkqtplot_test.depends = jkqtplotterlib

jkqtplotter_simpletest.file = test/simpletest/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib

test_multiplot.file = test/test_multiplot/test_multiplot.pro
test_multiplot.depends = jkqtplotterlib

defineTest(addSimpleTest) {
    test_name = $$1
    SUBDIRS += jkqtplotter_simpletest_$${test_name}

    jkqtplotter_simpletest_$${test_name}.file = test/simpletest_$${test_name}/jkqtplotter_simpletest_$${test_name}.pro
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
addSimpleTest(parametriccurve)
addSimpleTest(parsedfunctionplot)
addSimpleTest(functionplot)
#addSimpleTest(imageplot_nodatastore)
#addSimpleTest(rgbimageplot_opencv)
#addSimpleTest(imageplot_opencv)
