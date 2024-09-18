cmake_minimum_required(VERSION 3.23)

add_custom_target(JKQTMathText_GenerateDocImages
    COMMENT "Building JKQTMathText Documentation Images ..."
)

add_custom_target(JKQTPlotter_GenerateDocScreenshots
    COMMENT "Building JKQTPlotter Documentation Screenshots ..."
)

add_custom_target(JKQTPlotter_GenerateDocImages
    COMMENT "Building JKQTPlotter Documentation Images ..."
)


add_custom_target(JKQTP_GenerateAllDocImages
    DEPENDS JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocScreenshots JKQTMathText_GenerateDocImages
)

if(JKQtPlotter_BUILD_TOOLS)

    # custom target that generates the images for the JKQTMathText documentation
    set(JKQTMathText_GenerateDocImages_DefaultOptions --resolution=96 --fontsize=16 --fontmathroman=XITS --background=white --sizeincrease=5)
    set(JKQTMathText_GenerateDocImages_StdInputFiles
        jkqtmathtext_docimages_accents.jkmt
        jkqtmathtext_docimages_boxes.jkmt
        jkqtmathtext_docimages_braces.jkmt
        jkqtmathtext_docimages_formating.jkmt
        jkqtmathtext_docimages_frac.jkmt
        jkqtmathtext_docimages_math.jkmt
        jkqtmathtext_docimages_matrix.jkmt
        jkqtmathtext_docimages_misc.jkmt
        jkqtmathtext_docimages_specialfonts.jkmt
        jkqtmathtext_docimages_symbolsandencoding.jkmt
        jkqtmathtext_docimages_spaces.jkmt
    )


    set(JKQTMathText_GenerateDocImages_COMMAND $<TARGET_FILE:jkqtmathtext_render> ${JKQTMathText_GenerateDocImages_DefaultOptions})
    set(JKQTMathText_GenerateDocImages_OutputDir "${CMAKE_CURRENT_LIST_DIR}/images/jkqtmathtext/")

    foreach(file ${JKQTMathText_GenerateDocImages_StdInputFiles})
        string(REGEX MATCH ".*_([^_\.]+)\.[A-Za-z]+" dummy ${file})
        set(loc_name ${CMAKE_MATCH_1})
        string(TOUPPER ${loc_name} loc_name_upper ${file})
        set(loc_target_name "JKQTMathText_GenerateDocImages_${loc_name}")

        add_custom_target(${loc_target_name}
            COMMENT "Building JKQTMathText Documentation Images: ${loc_name_upper}"
            COMMAND ${JKQTMathText_GenerateDocImages_COMMAND} --inputfile="${CMAKE_CURRENT_LIST_DIR}/${file}" --outputdir="${JKQTMathText_GenerateDocImages_OutputDir}"
            WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
            DEPENDS jkqtmathtext_render
        )

        add_dependencies(JKQTMathText_GenerateDocImages ${loc_target_name})

        unset(loc_name)
        unset(loc_target_name)
        unset(loc_name_upper)
        unset(dummy)
    endforeach()

    add_custom_target(JKQTMathText_GenerateDocImages_listsymbols
        COMMENT "Building JKQTMathText Documentation Images: LISTSYMBOLS"
        COMMAND ${JKQTMathText_GenerateDocImages_COMMAND} --listsymbolsfonts=XITS_MATHANDTEXT,FIRA_MATHANDTEXT --listsymbols="${CMAKE_CURRENT_LIST_DIR}/dox/jkqtmathtext_supportedlatexsymbols.dox" --outputdir="${JKQTMathText_GenerateDocImages_OutputDir}/symbols/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtmathtext_render
    )

    add_dependencies(JKQTMathText_GenerateDocImages JKQTMathText_GenerateDocImages_listsymbols)
endif()











if(JKQtPlotter_BUILD_TOOLS)

    # custom target that generates the images for the JKQTPlotter documentation
    set(JKQTPlotter_GenerateDocImages_DefaultOptions )


    set(JKQTPlotter_GenerateDocImages_COMMAND $<TARGET_FILE:jkqtplotter_doc_imagegenerator> ${JKQTPlotter_GenerateDocImages_DefaultOptions})
    set(JKQTPlotter_GenerateDocImages_OutputDir "${CMAKE_CURRENT_LIST_DIR}/images/")
    set(JKQTPlotter_GenerateDocImages_dox "${CMAKE_CURRENT_LIST_DIR}/dox/")


    add_custom_target(JKQTPlotter_GenerateDocImages_listplotsymbols
        COMMENT "Building JKQTPlotter Documentation Images: LISTPLOTSYMBOLS"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listsymbols --iconsize=32 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/symbols/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listplotsymbols)

    add_custom_target(JKQTPlotter_GenerateDocImages_listlinedecorators
        COMMENT "Building JKQTPlotter Documentation Images: LISTLINEDECORATORS"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listlinedecorators --iconsize=32 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/linedecorators/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listlinedecorators)

    add_custom_target(JKQTPlotter_GenerateDocImages_listerrorindicators
        COMMENT "Building JKQTPlotter Documentation Images: listerrorindicators"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listerrorindicators --iconsize=32 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/errorindicators/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listerrorindicators)

    add_custom_target(JKQTPlotter_GenerateDocImages_listpalettes
        COMMENT "Building JKQTPlotter Documentation Images: listpalettes"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listpalettes --iconsize=256 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/palettes/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listpalettes)

    add_custom_target(JKQTPlotter_GenerateDocImages_listaxisstyleprops
        COMMENT "Building JKQTPlotter Documentation Images: listaxisstyleprops"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listaxisstyleprops --background=lightgrey --iconsize=50 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/axisstyle/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listaxisstyleprops)

    add_custom_target(JKQTPlotter_GenerateDocImages_listaxisticklabelangles
        COMMENT "Building JKQTPlotter Documentation Images: listaxisticklabelangles"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listaxisticklabelangles --background=lightgrey --iconsize=50 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/axisstyle/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listaxisticklabelangles)

    add_custom_target(JKQTPlotter_GenerateDocImages_listlabelpositions
        COMMENT "Building JKQTPlotter Documentation Images: listlabelpositions"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listlabelpositions --background=lightgrey --iconsize=50 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/labelstyles/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listlabelpositions)

    add_custom_target(JKQTPlotter_GenerateDocImages_liststyles
        COMMENT "Building JKQTPlotter Documentation Images: liststyles"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --liststyles --iconsize=1200 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/styles/" --doctomodify="${JKQTPlotter_GenerateDocImages_dox}/jkqtplotter_styling.dox"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_liststyles)

    add_custom_target(JKQTPlotter_GenerateDocImages_listkeypos
        COMMENT "Building JKQTPlotter Documentation Images: listkeypos"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listkeypos --iconsize=225 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/keypositions/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listkeypos)

    add_custom_target(JKQTPlotter_GenerateDocImages_listkeylayout
        COMMENT "Building JKQTPlotter Documentation Images: listkeylayout"
        COMMAND ${JKQTPlotter_GenerateDocImages_COMMAND} --listkeylayout --iconsize=225 --outputdir="${JKQTPlotter_GenerateDocImages_OutputDir}/keylayouts/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtplotter_doc_imagegenerator
    )
    add_dependencies(JKQTPlotter_GenerateDocImages JKQTPlotter_GenerateDocImages_listkeylayout)

endif(JKQtPlotter_BUILD_TOOLS)




if(JKQtPlotter_BUILD_EXAMPLES)

    # custom target that generates the images for the JKQTMathText documentation
    set(JKQTPlotter_GenerateDocScreenshots_DefaultOptions --screenshotdir="${CMAKE_CURRENT_LIST_DIR}/../screenshots/" --screenshot --disablehighdpi --smallscreenshotplot)
    # list examples for which to save a screenshot
    # the output basename equals the example name in this list,
    # unless you specify the basenames by adding it/them after a slash '/'
    # e.g. "barchart/barchart,barchart_hor"
    # the basenames are applied to the windows in the order of their windowTitle()s!
    # after a second slash '/' you can add extra command-line commands to send to the application
    # if you want to call any example twice (e.g. with differnt parameters), you need to give it a new name, but also the name of the actual executible in [..]:
    #      multithreaded/multithreaded/--mdfile=${CMAKE_CURRENT_LIST_DIR}/../examples/multithreaded/README.md
    #      "multithreaded_complex[multithreaded]/multithreaded_complex/--mdfile=${CMAKE_CURRENT_LIST_DIR}/../examples/multithreaded/README.md --complexlabel"
    set(JKQTPlotter_GenerateDocScreenshots_From
        scatter/scatter,scatter_error
        scatter_customsymbol
        simpletest
        barchart/barchart,barchart_hor
        barchart_twocolor/barchart_twocolor,barchart_twocolor_hor
        barchart_errorbars/barchart_errorbars,barchart_errorbars_hor
        barchart_functorfill/barchart_functorfill,barchart_functorfill_hor
        barchart_customdrawfunctor/barchart_customdrawfunctor,barchart_customdrawfunctor_hor
        wiggleplots/wiggleplot_x,wiggleplot_y
        advplotstyling/advancedlineandfillstyling
        boxplot
        contourplot
        violinplot/violinplot_hor,violinplot_vert
        errorbarstyles
        dateaxes/dateaxes,dateaxes_timeaxis
        distributionplot/test_distributionplot
        evalcurve
        filledgraphs/filledgraphs,filledgraphs_y
        filledgraphs_errors/filledgraphs_errors,filledgraphs_errors_y
        functionplot/functionplot,functionplot_fy
        styledboxplot/test_styledboxplot
        multiplot/multiplot,multiplot_controlwindow
        symbols_and_styles
        symbols_and_errors
        stepplots
        stackedbars/stackedbars,stackedbars_hor
        geo_arrows
        geo_simple
        geometric
        imageplot/imageplot,imageplot__scale02,imageplot__smallscalelimitcolor,imageplot__smallscalecolor,imageplot__smallscaletransparent/--iteratefunctorsteps
        imageplot_modifier
        imageplot_nodatastore
        imageplot_userpal/imageplot_userpal_program
        impulsesplot/impulsesplot,impulsesplot_symbols,impulsesplot_horizontal,errorbarstyles_impulses,errorbarstyles_impulses_horizontal
        parametriccurve/parametriccurve1,parametriccurve2
        paramscatterplot
        paramscatterplot_customsymbol
        #paramscatterplot_image
        parsedfunctionplot
        rgbimageplot
        rgbimageplot_qt/rgbimageplot_qt,rgbimageplot_qt_upsidedown/--iteratefunctorsteps
        mandelbrot
        geo_coordinateaxis0
        second_axis/second_axis,second_axis_hor
        logaxes/logaxes,logaxes_nolog,logaxes_nominorgrid/--iteratefunctorsteps
        #speed
        datastore/datastore,datastore_sine,datastore_linkedcarray,datastore_map,datastore_calccolumns,datastore_image,datastore_image_sorted,datastore_sineimg/--iteratefunctorsteps
        datastore_iterators/datastore_iterators,datastore_iterators_cosine,datastore_iterators_cosine_ysorted,datastore_iterators_cosine_yreplaced,/--iteratefunctorsteps
        datastore_groupedstat/datastore_groupedstat,datastore_groupedstat_barchartrawdata,datastore_groupedstat_barchart,datastore_groupedstat_boxplot,datastore_groupedstat_scatterrawdata,datastore_groupedstat_scatter/--iteratefunctorsteps
        datastore_regression/datastore_regression,datastore_regression_lin,datastore_regression_linrobust,datastore_regression_linrobust_p,datastore_regression_linweight,datastore_regression_nonlinreg_exp,datastore_regression_nonlinreg_pow,datastore_regression_polynom,datastore_regression_polynom_errros/--iteratefunctorsteps
        datastore_statistics/datastore_statistics,datastore_statistics_dataonly,datastore_statistics_boxplots_simple,datastore_statistics_boxplots_outliers,datastore_statistics_hist,datastore_statistics_kde,datastore_statistics_cumhistkde/--iteratefunctorsteps
        datastore_statistics_2d/datastore_statistics_2d
        multithreaded/multithreaded/--mdfile=${CMAKE_CURRENT_LIST_DIR}/../examples/multithreaded/README.md
        #"multithreaded_complex[multithreaded]/multithreaded_complex/--mdfile=${CMAKE_CURRENT_LIST_DIR}/../examples/multithreaded/README.md --complexlabel"
        graphlabels/graphlabels,graphlabels_hor
        vectorfield
        paramvectorfield
        financialgraphs
        geo_bezier
        ui_bind_scrollbar
    )





    # custom target that generates the images for the JKQTMathText documentation
    set(JKQTPlotter_GenerateDocImage_DefaultOptions --screenshotdir="${CMAKE_CURRENT_LIST_DIR}/../doc/images/" --screenshotplot --disablehighdpi)
    set(JKQTPlotter_GenerateDocImages_From
        stackedbars/JKQTPbarVerticalGraphStacked,JKQTPbarHorizontalGraphStacked/--smallscreenshotplot
        scatter/JKQTPXYScatterGraph,JKQTPXYScatterErrorGraph/--smallscreenshotplot
        simpletest/JKQTPXYLineGraph/--smallscreenshotplot
        barchart/JKQTPBarVerticalGraph,JKQTPBarHorizontalGraph,JKQTPBarVerticalGraphNoBaseline,JKQTPBarHorizontalGraphNoBaseline,JKQTPBarVerticalGraphBaseline,JKQTPBarHorizontalGraphBaseline,JKQTPBarVerticalAutoscaleShrinkOnly,JKQTPBarHorizontalAutoscaleShrinkOnly,JKQTPBarVerticalAutoscaleMaxWidthOnly,JKQTPBarHorizontalAutoscaleMaxWidthOnly/--iteratefunctorsteps--smallscreenshotplot
        barchart_twocolor/JKQTPBarVerticalGraphTwoColorFilling,JKQTPBarHorizontalGraphTwoColorFilling/--smallscreenshotplot
        barchart_errorbars/JKQTPBarVerticalErrorGraph,JKQTPBarHorizontalErrorGraph/--smallscreenshotplot
        barchart_functorfill/JKQTPBarVerticalGraphFunctorFilling,JKQTPBarHorizontalGraphFunctorFilling/--smallscreenshotplot
        barchart_customdrawfunctor/JKQTPBarVerticalGraphCustomDrawFunctor,JKQTPBarHorizontalGraphCustomDrawFunctor/--smallscreenshotplot
        wiggleplots/JKQTPFilledCurveXGraph_wiggle,JKQTPFilledCurveYGraph_wiggle
        contourplot/JKQTPColumnContourPlot/--smallscreenshotplot
        filledgraphs/JKQTPFilledCurveXGraph,JKQTPFilledCurveYGraph/--smallscreenshotplot
        filledgraphs_errors/JKQTPFilledCurveXErrorGraph,JKQTPFilledCurveYErrorGraph/--smallscreenshotplot
        evalcurve/JKQTPXYFunctionLineGraph/--smallscreenshotplot
        geo_coordinateaxis0/JKQTPCoordinateAxisStyleDrawMode0
        impulsesplot/JKQTPImpulsesVerticalGraph,JKQTPImpulsesVerticalGraph_Symbols,JKQTPImpulsesHorizontalGraph,JKQTPImpulsesVerticalErrorGraph,JKQTPImpulsesHorizontalErrorGraph,JKQTPImpulsesVerticalGraphNoBaseline,JKQTPImpulsesVerticalGraph_SymbolsNoBaseline,JKQTPImpulsesHorizontalGraphNoBaseline,JKQTPImpulsesVerticalErrorGraphNoBaseline,JKQTPImpulsesHorizontalErrorGraphNoBaseline,JKQTPImpulsesVerticalGraphBaseline,JKQTPImpulsesVerticalGraph_SymbolsBaseline,JKQTPImpulsesHorizontalGraphBaseline,JKQTPImpulsesVerticalErrorGraphBaseline,JKQTPImpulsesHorizontalErrorGraphBaseline/--iteratefunctorsteps--smallscreenshotplot
        symbols_and_errors/JKQTPXYLineErrorGraph_JKQTPErrorBars,JKQTPXYLineErrorGraph_JKQTPErrorLines,JKQTPXYLineErrorGraph_JKQTPErrorPolygons/--iteratefunctorsteps--iteratefunctorsteps_suppressinitial--smallscreenshotplot
        boxplot/JKQTPBoxplotVerticalGraph,JKQTPBoxplotHorizontalGraph/--iteratefunctorsteps--iteratefunctorsteps_suppressinitial--smallscreenshotplot
        second_axis/JKQTBasePlotter_addSecondaryYAxis,JKQTBasePlotter_addSecondaryXAxis
        graphlabels/JKQTPGLabelAwayFromXAxis,JKQTPGLabelAwayFromYAxis,JKQTPGLabelTowardsXAxis,JKQTPGLabelTowardsYAxis,JKQTPGLabelAboveData,JKQTPGLabelRightHandSide,JKQTPGLabelBelowData,JKQTPGLabelLeftHandSide,JKQTPGLabelCenteredOnData,JKQTPGLabelCenteredOnDataVertical,JKQTPGLSimpleBox,JKQTPGLSimpleBoxVertical,JKQTPGLSimpleBoxAndLine,JKQTPGLSimpleBoxAndLineVertical,JKQTPGLSimpleBoxAndLineONLYLABELS,JKQTPGLSimpleBoxAndLineONLYLABELSVertical,JKQTPGLabelHalfwaysToXAxis,JKQTPGLabelHalfwaysToYAxis/--iteratefunctorsteps--smallscreenshotplot
        vectorfield/JKQTPVectorFieldGraph,JKQTPVectorFieldGraphAnchorBottom,JKQTPVectorFieldGraphAnchorMid,JKQTPVectorFieldGraphAnchorTip,JKQTPVectorFieldGraphAutoscaleLength,JKQTPVectorFieldGraphLengthFromData,JKQTPVectorFieldGraphIgnoreLength,JKQTPVectorFieldGraphIgnoreLengthAutoscaleLineWidthFromLength,JKQTPVectorFieldGraphAutoscaleLengthAutoscaleLineWidthFromLength/--iteratefunctorsteps
        paramvectorfield/JKQTPParametrizedVectorFieldGraph,JKQTPParametrizedVectorFieldGraphColorFromMagnitude,JKQTPParametrizedVectorFieldGraphColorFromAngle,JKQTPParametrizedVectorFieldGraphDefaultColor/--iteratefunctorsteps
        financialgraphs/JKQTPFinancialGraph,JKQTPFinancialGraphCandleStick,JKQTPFinancialGraphSetCandlestickTwoColor,JKQTPFinancialGraphSetCandlestickTwoColor2,JKQTPFinancialGraphSetCandlestickOneColor,JKQTPFinancialGraphOHLC,JKQTPFinancialGraphSetOHLCTwoColor,JKQTPFinancialGraphSidyBySide/--iteratefunctorsteps
        geo_bezier/JKQTPGeoBezierCurveGraphic,JKQTPGeoBezierCurveMath,JKQTPGeoBezierCurveLogMath,JKQTPGeoBezierCurveLogGraphic/--iteratefunctorsteps
    )









    foreach(ex ${JKQTPlotter_GenerateDocScreenshots_From})
        set(example ${ex})
        set(basename ${ex})
        string(REGEX MATCH "([^/]+)/([^/]+)" dummy ${ex})
        set(extra_command "")
        if(CMAKE_MATCH_1 STREQUAL "" OR CMAKE_MATCH_2 STREQUAL "")
            set(example ${ex})
            set(basename ${ex})
        else()
            set(example ${CMAKE_MATCH_1})
            set(basename ${CMAKE_MATCH_2})
            set(CMAKE_MATCH_3 "")
            string(REGEX MATCH "([^/]+)/([^/]*)/(.+)" dummy ${ex})
            if(NOT (CMAKE_MATCH_3 STREQUAL ""))
                set(example ${CMAKE_MATCH_1})
                set(basename ${CMAKE_MATCH_2})
                set(extra_command ${CMAKE_MATCH_3})
            endif()

        endif()

        if (NOT extra_command STREQUAL "")
            string(REPLACE "--" ";--" extra_command "${extra_command}")
        endif()

        set(example_target ${example})
        string(REGEX MATCH "(.+)\\[(.+)\\]" dummy ${example})
        if(NOT (CMAKE_MATCH_2 STREQUAL ""))
            set(example ${CMAKE_MATCH_2})
            set(example_target ${CMAKE_MATCH_1})
            message(STATUS "example=${example}, example_target=${example_target}")
        endif()
        set(loc_target_name JKQTPlotter_GenerateDocScreenshots_${example_target})
        set(dep_name jkqtptest_${example})
        set(tgt_dependecies ${dep_name})
        if(NOT (CMAKE_MATCH_2 STREQUAL ""))
            set(tgt_dependecies jkqtptest_${example} JKQTPlotter_GenerateDocScreenshots_${example})
        endif()
        string(TOUPPER ${example_target} example_upper)
        add_custom_target(${loc_target_name}
            COMMENT "Building JKQTPlotter Documentation Screenshot: ${example_upper}"
            COMMAND $<TARGET_FILE:${dep_name}> ${JKQTPlotter_GenerateDocScreenshots_DefaultOptions} --screenshotbasename=${basename} ${extra_command} > c:/temp/${loc_target_name}.txt
            WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
            DEPENDS ${tgt_dependecies}
        )

        add_dependencies(JKQTPlotter_GenerateDocScreenshots ${loc_target_name})

        unset(loc_target_name)
        unset(dep_name)
        unset(example)
        unset(example_upper)
        unset(example_target)
        unset(basename)
        unset(tgt_dependecies)
    endforeach()


    foreach(ex ${JKQTPlotter_GenerateDocImages_From})
        string(REGEX MATCH "(.+)/(.+)" dummy ${ex})
        set(extra_command "")
        if(CMAKE_MATCH_1 STREQUAL "" OR CMAKE_MATCH_2 STREQUAL "")
            set(example ${ex})
            set(basename ${ex})
        else()
            set(example ${CMAKE_MATCH_1})
            set(basename ${CMAKE_MATCH_2})
            string(REGEX MATCH "(.+)/(.*)/(.+)" dummy ${ex})
            if(NOT (CMAKE_MATCH_3 STREQUAL ""))
                set(example ${CMAKE_MATCH_1})
                set(basename ${CMAKE_MATCH_2})
                set(extra_command ${CMAKE_MATCH_3})
            endif()

        endif()

        if (NOT extra_command STREQUAL "")
            string(REPLACE "--" ";--" extra_command "${extra_command}")
        endif()

        set(loc_target_name JKQTPlotter_GenerateDocImages_${example})
        set(dep_name jkqtptest_${example})
        string(TOUPPER ${example} example_upper)
        add_custom_target(${loc_target_name}
            COMMENT "Building JKQTPlotter Documentation Image: ${example_upper}"
            COMMAND $<TARGET_FILE:${dep_name}> ${JKQTPlotter_GenerateDocImage_DefaultOptions} --screenshotbasename=${basename} ${extra_command}
            WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
            DEPENDS ${dep_name}
        )

        add_dependencies(JKQTPlotter_GenerateDocImages ${loc_target_name})

        unset(loc_target_name)
        unset(dep_name)
        unset(example)
        unset(example_upper)
        unset(basename)
    endforeach()


endif()
