cmake_minimum_required(VERSION 3.0)

# Project Name and Version
project(JKQTPlotter LANGUAGES CXX VERSION 2019.11)


# set search path for CMake files
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Common Includes for JKQtPlotter
include(jkqtplotter_common_include)

# Project Name and Version
include(jkqtplotter_lib_properties)

# CMake options with default values and description texts
include(jkqtplotter_cmake_options)

# compiler settings for this lib
include(jkqtplotter_common_compilersettings)

# include Qt with appropriate options to build this lib
include(jkqtplotter_common_qtsettings)



# now add subdirectories with the library code ...
add_subdirectory(lib)
# ... and optionally the examples
if(JKQtPlotter_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



