cmake_minimum_required(VERSION 3.0)
find_package( CImg  )
if(CImg_FOUND)

    set(EXAMPLE_NAME rgbimageplot_cimg)
    set(EXENAME jkqtptest_${EXAMPLE_NAME})

    message( STATUS "..   Building Example ${EXAMPLE_NAME}" )

    # Set up source files
    set(SOURCES ${EXAMPLE_NAME}.cpp)
    set(HEADERS )
    set(RESOURCES  )
    set(UIS  )

    add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
    target_include_directories(${EXENAME} PRIVATE ../../lib)
    if(BUILD_STATIC_LIBS)
        target_link_libraries(${EXENAME} JKQTPlotterLib)
    elseif(BUILD_SHARED_LIBS)
        target_link_libraries(${EXENAME} JKQTPlotterSharedLib)
    endif()
    include_directories(${EXENAME}  ${CIMG_INCLUDE_DIR} )

    # Installation
    if(LIB_INSTALL)
        install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif(LIB_INSTALL)

    #Installation of Qt DLLs on Windows
    jkqtplotter_deployqt(${EXENAME})
        add_custom_command(TARGET ${EXENAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/rgbimageplot_cimg_example.bmp" $<TARGET_FILE_DIR:${EXENAME}>
        )
endif(CImg_FOUND)
