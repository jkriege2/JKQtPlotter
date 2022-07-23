function(jkqtplotter_deployopencv TARGET_NAME)
    if(OpenCV_FOUND)
        if(WIN32)
            if (EXISTS ${_OpenCV_LIB_PATH})
              add_custom_command(TARGET ${EXENAME} POST_BUILD        # Adds a post-build event the TARGET
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                      "${_OpenCV_LIB_PATH}"
                      "$<TARGET_FILE_DIR:${EXENAME}>"
                    )
            endif()
            if (EXISTS ${OpenCV_INSTALL_PATH}/bin)
              add_custom_command(TARGET ${EXENAME} POST_BUILD        # Adds a post-build event the TARGET
                      COMMAND ${CMAKE_COMMAND} -E copy_directory
                      "${OpenCV_INSTALL_PATH}/bin"
                      "$<TARGET_FILE_DIR:${EXENAME}>"
                    )
            endif()
        endif(WIN32)
     endif(OpenCV_FOUND)
endfunction(jkqtplotter_deployopencv)
