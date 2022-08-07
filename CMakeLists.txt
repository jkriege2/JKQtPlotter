cmake_minimum_required(VERSION 3.16)

# Project Name and Version
# starting from 2020: using semantic version number <major>.<minor>.<patch>
#   - bugfixes which do not change the API/ABI lead to an increase in <patch>
#   - backward-compatible API/ABI-changes mean an increasing <minor> version
#   - NON backward-compatible API/ABI-changes mean an increasing <major> version
# relation to older versioning scheme:
#   - v2015.10  <--> 1.0.0
#   - v2018.08  <--> 2.0.0
#   - v2019.11  <--> 3.x.y
#   ==> sematic versioning starts with 4.0.0
project(JKQTPlotter
          LANGUAGES CXX
          VERSION 5.0.0
          HOMEPAGE_URL https://github.com/jkriege2/JKQtPlotter
          DESCRIPTION "an extensive Qt5 & Qt6 Plotter framework (including a feature-richt plotter widget, a speed-optimized, but limited variant and a LaTeX equation renderer!), written fully in C/C++ and without external dependencies"
      )


# set search path for CMake files
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
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

if(JKQtPlotter_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

add_subdirectory(doc)



