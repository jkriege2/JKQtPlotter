cmake_minimum_required(VERSION 3.16)


if(JKQtPlotter_BUILD_TOOLS)

    # custom target that generates the images for the JKQTMathText documentation
    set(JKQTMathText_GenerateDocImages_DefaultOptions --resolution=96 --fontsize=15 --fontmathroman=XITS --background=white --sizeincrease=5)
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
    add_custom_target(JKQTMathText_GenerateDocImages
        COMMENT "Building JKQTMathText Documentation Images ..."
        DEPENDS jkqtmathtext_render
    )

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
        COMMAND ${JKQTMathText_GenerateDocImages_COMMAND} --listsymbols="${CMAKE_CURRENT_LIST_DIR}/dox/jkqtmathtext_supportedlatexsymbols.dox" --outputdir="${JKQTMathText_GenerateDocImages_OutputDir}/symbols/"
        WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
        DEPENDS jkqtmathtext_render
    )

    add_dependencies(JKQTMathText_GenerateDocImages JKQTMathText_GenerateDocImages_listsymbols)
endif()







if(JKQtPlotter_BUILD_EXAMPLES)

    # custom target that generates the images for the JKQTMathText documentation
    set(JKQTPlotter_GenerateDocScreenshots_DefaultOptions --screenshotdir="${CMAKE_CURRENT_LIST_DIR}/../screenshots/" --screenshot --disablehighdpi --smallscreenshotplot)
    # list examples for which to save a screenshot
    # the output basename equals the example name in this list,
    # unless you specify the basenames by adding it/them after a slash '/'
    # e.g. "barchart/barchart,barchart_hor"
    set(JKQTPlotter_GenerateDocScreenshots_From
        scatter
        simpletest
        barchart/barchart_hor,barchart
        wiggleplots/wiggleplot_x,wiggleplot_y
        advplotstyling/advancedlineandfillstyling
        boxplot
        contourplot
        violinplot/violinplot_hor,violinplot_vert
        errorbarstyles
        dateaxes/dateaxes,dateaxes_timeaxis
        distributionplot/test_distributionplot
        evalcurve
        filledgraphs
        functionplot/functionplot,functionplot_fy
        styledboxplot/test_styledboxplot
        test_multiplot
    )

    add_custom_target(JKQTPlotter_GenerateDocScreenshots
        COMMENT "Building JKQTPlotter Documentation Screenshots ..."
    )

    foreach(ex ${JKQTPlotter_GenerateDocScreenshots_From})
        string(REGEX MATCH "(.+)/(.+)" dummy ${ex})
        if(CMAKE_MATCH_1 STREQUAL "" OR CMAKE_MATCH_2 STREQUAL "")
            set(example ${ex})
            set(basename ${ex})
        else()
            set(example ${CMAKE_MATCH_1})
            set(basename ${CMAKE_MATCH_2})
        endif()

        set(loc_target_name JKQTPlotter_GenerateDocScreenshots_${example})
        set(dep_name jkqtptest_${example})
        string(TOUPPER ${example} example_upper)
        add_custom_target(${loc_target_name}
            COMMENT "Building JKQTPlotter Documentation Screenshot: ${example_upper}"
            COMMAND $<TARGET_FILE:${dep_name}> ${JKQTPlotter_GenerateDocScreenshots_DefaultOptions} --screenshotbasename=${basename}
            WORKING_DIRECTORY ${JKQtPlotter_QT_BINDIR}
            DEPENDS ${dep_name}
        )

        add_dependencies(JKQTPlotter_GenerateDocScreenshots ${loc_target_name})

        unset(loc_target_name)
        unset(dep_name)
        unset(example)
        unset(example_upper)
        unset(basename)
    endforeach()

endif()
