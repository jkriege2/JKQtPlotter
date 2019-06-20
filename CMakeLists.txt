cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)



project(JKQTPlotter LANGUAGES CXX)



if(NOT DEFINED BUILD_SHARED_LIBS)
    option(BUILD_SHARED_LIBS "Build as shared library" ON)
endif()
if(NOT DEFINED BUILD_STATIC_LIBS)
    option(BUILD_STATIC_LIBS "Build as static library" ON)
endif()
if(NOT DEFINED BUILD_INCLUDE_XITS_FONTS)
    option(BUILD_INCLUDE_XITS_FONTS "Include XITS fonts as resources in library" ON)
endif()
if(NOT DEFINED BUILD_EXAMPLES)
    option(BUILD_EXAMPLES "Build examples" ON)
endif()
if(NOT DEFINED BUILD_HAS_OPENCV)
    option(BUILD_HAS_OPENCV "OpenCV available?" OFF)
endif()
if(NOT DEFINED LIB_INSTALL)
    option(LIB_INSTALL "Install library" ON)
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    option(CMAKE_INSTALL_PREFIX "Install directory" ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif()






include(CheckCXXCompilerFlag)
if (NOT WIN32)
    include(GNUInstallDirs)
endif(NOT WIN32)

# Instruct CMake to run moc+rcc+uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build" FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if (NOT CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "bin")
endif()

if (NOT CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()


find_package(Qt5 5.0 REQUIRED Core Gui Widgets PrintSupport Svg Xml OpenGl)



set (CMAKE_CXX_STANDARD 11)

if(MSVC)
    if(BUILD_SHARED_LIBS)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    
    add_compile_options(/EHsc)
    # To enable M_PI, M_E,...
    add_definitions(/D_USE_MATH_DEFINES)
    # To Prevent Errors with min() and max()
    add_definitions(/DNOMINMAX)
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    add_definitions(/D_CRT_NO_VA_START_VALIDATION)
else()
    add_compile_options(-fexceptions)
endif()

if(BUILD_INCLUDE_XITS_FONTS)
    add_definitions(-DAUTOLOAD_XITS_FONTS -DUSE_XITS_FONTS )
else()
    add_definitions(-DNO_XITS_FONTS )
endif()



# place all DLLs and EXEs in the subdirectory output of the top level directory of the build tree
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)



add_subdirectory(lib)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



