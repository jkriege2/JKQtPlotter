function(jkqtplotter_deployqt TARGET_NAME)
    #use windeploy
    if (WIN32)
        get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
        find_program(WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${_qt_bin_dir}")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
          find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt.debug.bat HINTS "${_qt_bin_dir}")
        else()
          find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${_qt_bin_dir}")
        endif()        



        # install system runtime lib
        include( InstallRequiredSystemLibraries )
        if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
            install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT System )
        endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )

        get_filename_component(CMAKE_CXX_COMPILER_BINPATH ${CMAKE_CXX_COMPILER} DIRECTORY )
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_ENV_SETUP}" && "${WINDEPLOYQT_EXECUTABLE}" \"$<TARGET_FILE:${TARGET_NAME}>\"
            COMMENT "Running windeployqt ... "
        )
    endif(WIN32)
endfunction(jkqtplotter_deployqt)
