TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
            jkqtmathtext_simpletest \
            jkqtplot_test \
            jkqtplotter_simpletest \


jkqtplotterlib.file = lib/jkqtplotterlib.pro

jkqtmathtext_simpletest.subdir = test/jkqtmathtext_simpletest
jkqtmathtext_simpletest.depends = jkqtplotterlib

jkqtmathtext_test.subdir = test/jkqtmathtext_test
jkqtmathtext_test.depends = jkqtplotterlib

jkqtplot_test.subdir = test/jkqtplot_test
jkqtplot_test.depends = jkqtplotterlib

jkqtplotter_simpletest.file = test/simpletest/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib

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
addSimpleTest(speed)
addSimpleTest(rgbimageplot_qt)
#addSimpleTest(imageplot_nodatastore)
#addSimpleTest(rgbimageplot_opencv)
#addSimpleTest(imageplot_opencv)
