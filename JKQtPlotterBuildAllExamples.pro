TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib \
            jkqtplotterlib_sharedlib \
            jkqtmathtextlib \
            jkqtmathtextlib_sharedlib \
            jkqtpcommonlib \
            jkqtpcommonlib_sharedlib \
            jkqtfastplotterlib \
            jkqtfastplotterlib_sharedlib \
            jkqtmathtext_simpletest \
            jkqtplot_test \
            jkqtplotter_simpletest \
            test_multiplot \
            jkqtfastplotter_test


jkqtplotterlib.file = qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro
jkqtplotterlib_sharedlib.file = qmake/sharedlib/jkqtplotterlib/jkqtplotterlib.pro

jkqtmathtextlib.file = qmake/staticlib/jkqtmathtextlib/jkqtmathtextlib.pro
jkqtmathtextlib_sharedlib.file = qmake/sharedlib/jkqtmathtextlib/jkqtmathtextlib.pro

jkqtpcommonlib.file = qmake/staticlib/jkqtpcommonlib/jkqtpcommonlib.pro
jkqtpcommonlib_sharedlib.file = qmake/sharedlib/jkqtpcommonlib/jkqtpcommonlib.pro

jkqtfastplotterlib.file = qmake/staticlib/jkqtfastplotterlib/jkqtfastplotterlib.pro
jkqtfastplotterlib_sharedlib.file = qmake/sharedlib/jkqtfastplotterlib/jkqtfastplotterlib.pro

jkqtmathtext_simpletest.subdir = examples/jkqtmathtext_simpletest
jkqtmathtext_simpletest.depends = jkqtmathtextlib 

jkqtmathtext_test.subdir = examples/jkqtmathtext_test
jkqtmathtext_test.depends = jkqtplotterlib

jkqtplot_test.subdir = examples/jkqtplot_test
jkqtplot_test.depends = jkqtplotterlib

jkqtplotter_simpletest.file = examples/simpletest/jkqtplotter_simpletest.pro
jkqtplotter_simpletest.depends = jkqtplotterlib

jkqtfastplotter_test.file = $$PWD/examples/jkqtfastplotter_test/jkqtfastplotter_test.pro
jkqtfastplotter_test.depends = jkqtfastplotterlib

defineTest(addSimpleTest) {
    test_name = $$1
    SUBDIRS += jkqtptst_$${test_name}

    jkqtptst_$${test_name}.file = examples/$${test_name}/$${test_name}.pro
    jkqtptst_$${test_name}.depends = jkqtplotterlib

    export (jkqtptst_$${test_name}.file)
    export (jkqtptst_$${test_name}.depends)

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
addSimpleTest(rgbimageplot)
addSimpleTest(rgbimageplot_qt)
addSimpleTest(impulsesplot)
addSimpleTest(paramscatterplot)
addSimpleTest(paramscatterplot_image)
addSimpleTest(parametriccurve)
addSimpleTest(parsedfunctionplot)
addSimpleTest(functionplot)
addSimpleTest(geometric)
addSimpleTest(ui)
addSimpleTest(boxplot)
addSimpleTest(advancedlineandfillstyling)
addSimpleTest(imageplot_nodatastore)
addSimpleTest(datastore)
addSimpleTest(datastore_iterators)
addSimpleTest(datastore_statistics)
addSimpleTest(datastore_statistics_2d)
addSimpleTest(datastore_regression)
addSimpleTest(datastore_groupedstat)
addSimpleTest(contourplot)
addSimpleTest(violinplot)
#addSimpleTest(rgbimageplot_opencv)
#addSimpleTest(imageplot_opencv)



defineTest(addTest) {
    test_name = $$1
    SUBDIRS += test_$${test_name}

    test_$${test_name}.file = examples/$${test_name}/test_$${test_name}.pro
    test_$${test_name}.depends = jkqtplotterlib

    export (test_$${test_name}.file)
    export (test_$${test_name}.depends)

    export (SUBDIRS)
}

addTest(multiplot)
addTest(user_interaction)
addTest(styling)
addTest(styledboxplot)
addTest(distributionplot)

