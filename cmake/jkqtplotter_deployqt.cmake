function(jkqtplotter_deployqt TARGET_NAME)
    #use windeploy
    if (WIN32)
        get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

        set(WINDEPLOYQTOPTION "--release")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(WINDEPLOYQTOPTION "--debug")
        endif()

        # install system runtime lib
        include( InstallRequiredSystemLibraries )
        if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
            install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT System )
        endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )

        get_filename_component(CMAKE_CXX_COMPILER_BINPATH ${CMAKE_CXX_COMPILER} DIRECTORY )
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${CMAKE_CXX_COMPILER_BINPATH}\;${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                    --compiler-runtime
                    ${WINDEPLOYQTOPTION}
                    \"$<TARGET_FILE:${TARGET_NAME}>\"
            COMMENT "Running windeployqt ... "
        )
    endif(WIN32)
endfunction(jkqtplotter_deployqt)