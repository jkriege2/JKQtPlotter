# - Try to find CImg lib
#
#  CIMG_FOUND - system has CImg lib
#  CIMG_INCLUDE_DIR - the CImg include directory


macro(_cimg_check_path)

  if(EXISTS "${CIMG_INCLUDE_DIR}/CImg.h")
    set(CIMG_FOUND TRUE)
  endif()

  if(NOT CIMG_FOUND)
    message(STATUS "CImg include path was specified but no CImg.h file was found: ${CIMG_INCLUDE_DIR}")
  endif()

endmacro()

if(NOT CIMG_INCLUDE_DIR)
  message(STATUS "CImg: trying to locate CImg library")
  find_path(CIMG_INCLUDE_DIR NAMES CImg.h
	PATHS
	${CImg_DIR}
	${CImg_DIR}/include
	${CImg_DIR}/cimg
	${CImg_DIR}/include/cimg
	${CImg_DIR}/cimg/include
	${CIMG_DIR}
	${CIMG_DIR}/include
	${CIMG_DIR}/cimg
	${CIMG_DIR}/include/cimg
	${CIMG_DIR}/cimg/include
	${CMAKE_INSTALL_PREFIX}
	${CMAKE_INSTALL_PREFIX}/cimg
	${CMAKE_INSTALL_PREFIX}/include
	${CMAKE_INSTALL_PREFIX}/include/cimg
	${CMAKE_INSTALL_PREFIX}/cimg/include
  ${CIMG_PKGCONF_INCLUDE_DIRS}
	/usr/local/include
  /usr/include
  ${KDE4_INCLUDE_DIR}
	${KDE4_INCLUDE_DIR}/cimg
	${KDE4_INCLUDE_DIR}/include
	${KDE4_INCLUDE_DIR}/include/cimg
	${KDE4_INCLUDE_DIR}/cimg/include
  )

endif()

if(CIMG_INCLUDE_DIR)
  _cimg_check_path()
endif()


if (CIMG_FOUND)
  list(APPEND CIMG_INCLUDE_DIRS
    ${CIMG_INCLUDE_DIR}
  )

  ### DISPLAY :: X11 on unix-based system and GDI on windows ###
    if (UNIX OR APPLE)
        find_package (X11 QUIET) # xshm xrandr are detected as well
        if (X11_FOUND)
            set (CIMG_INCLUDE_DIRS ${CIMG_INCLUDE_DIRS} ${X11_INCLUDE_DIR} )
            set (CIMG_LIBRARIES ${CIMG_LIBRARIES} ${X11_LIBRARIES})
            message (STATUS "FindCImg.cmake: X11 found.")
            
    ### X11 extension :: XSHM ###
            if (X11_XShm_FOUND)
                set (CIMG_DEFINITIONS ${CIMG_DEFINITIONS} -Dcimg_use_xshm)
                message(STATUS "FindCImg.cmake: xshm found")
            else (X11_XShm_FOUND)
                message(STATUS "!!! FindCIMG.cmake !!! xshm NOT found.")
            endif (X11_XShm_FOUND)
    
    ### X11 extension :: XRANDR ###
            if (X11_Xrandr_FOUND)
                set (CIMG_DEFINITIONS ${CIMG_DEFINITIONS} -Dcimg_use_xrandr)
                set (CIMG_LIBRARIES ${CIMG_LIBRARIES} ${X11_Xrandr_LIB})
                message(STATUS "FindCImg.cmake: xrandr found")
            else (X11_Xrandr_FOUND)
                message(STATUS "!!! FindCIMG.cmake !!! xrandr NOT found")
            endif (X11_Xrandr_FOUND)
            
    ### PThread is required when using X11 display engine ###
            find_package (Threads QUIET)
            if (Threads_FOUND)
                set (CIMG_INCLUDE_DIRS ${CIMG_INCLUDE_DIRS})
                set (CIMG_LIBRARIES ${CIMG_LIBRARIES} Threads::Threads)
                message(STATUS "FindCImg.cmake: pthread found")
            else (PTHREAD_FOUND)
                message(STATUS "!!! FindCIMG.cmake !!! pthread NOT found. pthread required by cimg for running X11.")
                message(FATAL_ERROR "You need a display engine such as X11 (linux, macosx) or GDI (windows) to compile this program. Please install libs and developpement headers")
            endif (PTHREAD_FOUND)
            
        else (X11_FOUND)
            message (STATUS "!!! FindCIMG.cmake !!! X11 NOT found.")
            message(WARNING "FindCImg.cmake: display disabled")
            set (CIMG_DEFINITIONS ${CIMG_DEFINITIONS} -Dcimg_display=0)
        endif (X11_FOUND)
    else (UNIX OR APPLE)
        if (WIN32)
           find_library(GDI_LIBRARY gdi32)
           if (GDI_LIBRARY)
                set (CIMG_LIBRARIES ${CIMG_LIBRARIES} ${GDI_LIBRARY})
                message(STATUS "FindCImg.cmake: GDI found")
            else (GDI_LIBRARY)
                message(STATUS "!!! FindCIMG.cmake !!! GDI NOT found.")
                message(WARNING "FindCImg.cmake: display disabled")
                set (CIMG_DEFINITIONS ${CIMG_DEFINITIONS} -Dcimg_display=0)
            endif (GDI_LIBRARY)
        endif (WIN32)
    endif (UNIX OR APPLE)

endif()

set (CIMG_INCLUDE_DIRS ${CIMG_INCLUDE_DIRS} CACHE STRING "include directories for cimg dependancies")
set (CIMG_LIBRARIES ${CIMG_LIBRARIES} CACHE STRING "cimg required and optional 3rd party libraries")
set (CIMG_DEFINITIONS ${CIMG_DEFINITIONS} CACHE STRING "cimg_use_xxx defines")
set (CIMG_C_FLAGS ${CIMG_C_FLAGS}  CACHE STRING "c flags for cimg")
set (CIMG_CXX_FLAGS ${CIMG_CXX_FLAGS} CACHE STRING "c++ flags for cimg")



include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CImg DEFAULT_MSG CIMG_INCLUDE_DIR CIMG_FOUND)

mark_as_advanced(CIMG_INCLUDE_DIR)

if (CIMG_FOUND)
  # Library definition
  add_library(CImg::CImg INTERFACE IMPORTED)
   
  target_compile_options(CImg::CImg INTERFACE ${CIMG_C_FLAGS} ${CIMG_CXX_FLAGS})
  target_compile_definitions(CImg::CImg INTERFACE ${CIMG_DEFINITIONS})
  target_link_libraries(CImg::CImg INTERFACE ${CIMG_LIBRARIES})
  target_include_directories(CImg::CImg INTERFACE ${CIMG_INCLUDE_DIRS})
endif(CIMG_FOUND)
