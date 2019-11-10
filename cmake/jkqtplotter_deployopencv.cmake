function(jkqtplotter_deployopencv TARGET_NAME)
    #use windeploy
    if(OpenCV_FOUND)
		if(WIN32)
			add_custom_command(TARGET ${EXENAME} POST_BUILD        # Adds a post-build event the TARGET
				COMMAND ${CMAKE_COMMAND} -E copy_directory
				"${_OpenCV_LIB_PATH}"
				$<TARGET_FILE_DIR:${EXENAME}>
				)
		endif(WIN32)
	endif(OpenCV_Found)
endfunction(jkqtplotter_deployopencv)