function(jkqtplotter_deployqt TARGET_NAME)
    #use windeploy
    if (WIN32)
        # install system runtime lib
        include( InstallRequiredSystemLibraries )
        if( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )
            install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT System )
        endif( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS )

        get_filename_component(CMAKE_CXX_COMPILER_BINPATH ${CMAKE_CXX_COMPILER} DIRECTORY )
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E env PATH="${JKQtPlotter_QT_BINDIR}" "${JKQtPlotter_WINDEPLOYQT_EXECUTABLE}" \"$<TARGET_FILE:${TARGET_NAME}>\" --verbose 0 --no-translations --no-compiler-runtime
            COMMENT "Running windeployqt ... "
        )
    endif(WIN32)
endfunction(jkqtplotter_deployqt)
