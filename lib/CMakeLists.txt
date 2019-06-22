cmake_minimum_required(VERSION 3.0)



message( STATUS )
message( STATUS "=============================================================================" )
message( STATUS "== JKQTPlotterLib Build Information                                        ==" )
message( STATUS "=============================================================================" )
message( STATUS "Version:           ${PROJECT_VERSION}")
if (CMAKE_BUILD_TYPE)
    message( STATUS "Build type:       ${CMAKE_BUILD_TYPE}")
endif(CMAKE_BUILD_TYPE)
message( STATUS "Using compiler:    ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}" )
message( STATUS "Using Qt:          ${Qt5_VERSION}" )
message( STATUS "                     [at ${Qt5_DIR}]" )
if(BUILD_INCLUDE_XITS_FONTS)
    message( STATUS "Using XITS fonts" )
endif(BUILD_INCLUDE_XITS_FONTS)
message( STATUS "Installing to:     ${CMAKE_INSTALL_PREFIX}" )
message( STATUS "=============================================================================" )
message( STATUS )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/jkqtplotter_version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/jkqtplotter_version.h)


include_directories(.)

add_subdirectory(jkqtcommon)
add_subdirectory(jkqtmathtext)
add_subdirectory(jkqtfastplotter)
add_subdirectory(jkqtplotter)
