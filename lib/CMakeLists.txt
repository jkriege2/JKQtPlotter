cmake_minimum_required(VERSION 3.16)



message( STATUS )
message( STATUS "=============================================================================" )
message( STATUS "== JKQTPlotterLib Build Information                                        ==" )
message( STATUS "=============================================================================" )
message( STATUS "Version:           ${PROJECT_VERSION}")
if (CMAKE_BUILD_TYPE)
    message( STATUS "Build type:        ${CMAKE_BUILD_TYPE}")
endif(CMAKE_BUILD_TYPE)
message( STATUS "Using CMake:       ${CMAKE_VERSION}")
message( STATUS "        Generator: ${CMAKE_GENERATOR}")
message( STATUS "Using compiler:    ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, platform: ${CMAKE_CXX_PLATFORM_ID}" )
message( STATUS "C++ Standard:      C++${CMAKE_CXX_STANDARD}, required: ${CMAKE_CXX_STANDARD_REQUIRED}" )
message( STATUS "Architecture:      ${CMAKE_CXX_LIBRARY_ARCHITECTURE} / ${CMAKE_LIBRARY_ARCHITECTURE}" )
message( STATUS "System Name:       ${CMAKE_SYSTEM_NAME}" )
message( STATUS "Processor Name:    ${CMAKE_SYSTEM_PROCESSOR}" )
message( STATUS "Using Qt:          ${QT_VERSION}" )
message( STATUS "                     [at ${QT_DIR}]" )
if(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
    message( STATUS "Using XITS fonts" )
endif(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
message( STATUS "Installing to:     ${CMAKE_INSTALL_PREFIX}" )
message( STATUS "=============================================================================" )
message( STATUS )

configure_file(${CMAKE_CURRENT_LIST_DIR}/jkqtplotter_version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/jkqtplotter_version.h)


include_directories(.)

add_subdirectory(jkqtcommon)
add_subdirectory(jkqtmathtext)
add_subdirectory(jkqtfastplotter)
add_subdirectory(jkqtplotter)
