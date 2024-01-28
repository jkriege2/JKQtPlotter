include(CMakePackageConfigHelpers)

function(jkqtplotter_add_library lib_name)
    # create the library (SHARED/STATIC is controlled by the CMake option BUILD_SHARED_LIBS
    add_library(${lib_name} )
    # ... add an alias with the correct namespace
    add_library(${jkqtplotter_namespace}${lib_name} ALIAS ${lib_name})

    # set the libraries output name
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name}${JKQtPlotter_LIBNAME_ADDITION}")

endfunction()



function(jkqtplotter_setDefaultLibOptions TARGETNAME libBaseName PRECOMPHEADERFILE)
    string(TOUPPER ${libBaseName} libBasenameUPPER)

    # set library version
    set_property(TARGET ${TARGETNAME} PROPERTY VERSION "${PROJECT_VERSION}")
    # set required Cxx-Standard
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD ${JKQtPlotter_QT_CXX_STANDARD})
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD_REQUIRED ${JKQtPlotter_QT_CXX_STANDARD_REQUIRED})
    target_compile_features(${TARGETNAME} PUBLIC ${JKQtPlotter_QT_CXX_COMPILE_FEATURE})
    # set options, specific to shared libraries
    if (BUILD_SHARED_LIBS)
        target_compile_definitions(${TARGETNAME} PUBLIC ${libBasenameUPPER}_LIB_IN_DLL)
        target_compile_definitions(${TARGETNAME} PRIVATE ${libBasenameUPPER}_LIB_EXPORT_LIBRARY)
        set_target_properties(${TARGETNAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    endif(BUILD_SHARED_LIBS)

    if(MINGW)
      # COMPILER-SETTINGS FOR MINGW
      target_compile_options(${TARGETNAME} PUBLIC -fexceptions)
    elseif(MSVC)
      # COMPILER-SETTINGS FOR MS VISUAL C++
      target_compile_options(${TARGETNAME} PUBLIC /EHsc)
      target_compile_definitions(${TARGETNAME} PUBLIC NOMINMAX)
    endif()

    # enable TIMING INFO output (if activated by option JKQtPlotter_BUILD_WITH_TIMING_INFO_OUTPUT)
    if(JKQtPlotter_BUILD_WITH_TIMING_INFO_OUTPUT)
        target_compile_definitions(${TARGETNAME} PRIVATE JKQTBP_AUTOTIMER)
    endif()

    # set default include paths
    target_include_directories(${TARGETNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # precomiled headers to speed up compilation
    if (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS AND (NOT PRECOMPHEADERFILE EQUAL ""))
      target_precompile_headers(${TARGETNAME} PRIVATE ${PRECOMPHEADERFILE})
      target_sources(${TARGETNAME} PRIVATE ${PRECOMPHEADERFILE})
    endif()

endfunction()




include(GNUInstallDirs)




function(jkqtplotter_installlibrary_new lib_name libBasename libSrcDir)
    string(TOLOWER "${libBasename}" libIncludeSubdir)

    # export targets (including headers!)
    install(TARGETS ${lib_name}
        EXPORT ${lib_name}_TARGETS
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
    )
    # generate ...Config.cmake
    set(JKQTP_CURRENT_TARGET_FILENAME "${lib_name}Targets.cmake")
    configure_file(${libSrcDir}/LibTarget.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Config.cmake" @ONLY)
    # install export files
    install(EXPORT ${lib_name}_TARGETS
        FILE "${JKQTP_CURRENT_TARGET_FILENAME}"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${jkqtplotter_cmakeSubdir}
        NAMESPACE ${jkqtplotter_namespace}
    )

   # write CMake version file
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION}
                                     COMPATIBILITY AnyNewerVersion )

    #install PDB-files
    if(WIN32 AND MSVC AND (NOT MSVC_VERSION LESS 1600) AND (NOT CMAKE_VERSION VERSION_LESS "3.1") AND BUILD_SHARED_LIBS)
        install(FILES $<TARGET_PDB_FILE:${lib_name}> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)
    endif()

    # install ...Version.cmake
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake"
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${jkqtplotter_cmakeSubdir}  )
    # install ...Config.cmake
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Config.cmake"
                    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${jkqtplotter_cmakeSubdir}  )
    # install readme.txt
    configure_file(${PROJECT_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt" DESTINATION ${CMAKE_INSTALL_DOCDIR} )
    # install license
    install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION ${CMAKE_INSTALL_DOCDIR}) # RENAME "${lib_name}_LICENSE.txt" )
endfunction(jkqtplotter_installlibrary_new)


function(jkqtplotter_add_test TEST_NAME)

    set(EXENAME ${TEST_NAME})

    message( STATUS "..   Building Unit Test ${TEST_NAME}" )

    add_executable(${EXENAME} WIN32)
    target_include_directories(${EXENAME} PRIVATE ../../lib)
    target_link_libraries(${EXENAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test)

    target_sources(${EXENAME} PRIVATE ${TEST_NAME}.cpp)

    # Installation
    install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    add_test(NAME ${TEST_NAME} COMMAND COMMAND $<TARGET_FILE:${EXENAME}>)

    #Installation of Qt DLLs on Windows
    jkqtplotter_deployqt(${EXENAME})
endfunction(jkqtplotter_add_test)

function(jkqtplotter_add_jkqtcommmon_test TEST_NAME)
    jkqtplotter_add_test(${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE ${jkqtplotter_namespace}JKQTCommon${jkqtplotter_LIBNAME_VERSION_PART})
endfunction(jkqtplotter_add_jkqtcommmon_test)

function(jkqtplotter_add_jkqtmath_test TEST_NAME)
    jkqtplotter_add_test(${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE ${jkqtplotter_namespace}JKQTMath${jkqtplotter_LIBNAME_VERSION_PART})
endfunction(jkqtplotter_add_jkqtmath_test)

function(jkqtplotter_add_jkqtmathtext_test TEST_NAME)
    jkqtplotter_add_test(${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE ${jkqtplotter_namespace}JKQTMathText${jkqtplotter_LIBNAME_VERSION_PART})
endfunction(jkqtplotter_add_jkqtmathtext_test)

function(jkqtplotter_add_jkqtplotter_test TEST_NAME)
    jkqtplotter_add_test(${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE ${jkqtplotter_namespace}JKQTPlotter${jkqtplotter_LIBNAME_VERSION_PART})
endfunction(jkqtplotter_add_jkqtplotter_test)
