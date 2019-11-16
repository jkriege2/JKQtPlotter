# - Try to find CImg lib
#
#  CIMG_FOUND - system has CImg lib
#  CIMG_INCLUDE_DIR - the CImg include directory


macro(_cimg_check_path)

  if(EXISTS "${CIMG_INCLUDE_DIR}/CImg.h")
    set(CImg_FOUND TRUE)
  endif()

  if(NOT CImg_FOUND)
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
	${CMAKE_INSTALL_PREFIX}
	${CMAKE_INSTALL_PREFIX}/cimg
	${CMAKE_INSTALL_PREFIX}/include
	${CMAKE_INSTALL_PREFIX}/include/cimg
	${CMAKE_INSTALL_PREFIX}/cimg/include
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

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CImg DEFAULT_MSG CIMG_INCLUDE_DIR CImg_FOUND)

mark_as_advanced(CIMG_INCLUDE_DIR)