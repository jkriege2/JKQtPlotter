if(NOT DEFINED JKQtPlotter_BUILD_SHARED_LIBS)
    option(JKQtPlotter_BUILD_SHARED_LIBS "Build as shared library" ON)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_STATIC_LIBS)
    option(JKQtPlotter_BUILD_STATIC_LIBS "Build as static library" OFF)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
    option(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS "Include XITS fonts (SIL OPEN FONT LICENSE Version 1.1) as resources in library" ON)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_DECORATE_LIBNAMES_WITH_BUILDTYPE)
    option(JKQtPlotter_BUILD_DECORATE_LIBNAMES_WITH_BUILDTYPE "If set, the build-type (debug/release/...) is appended to the library name" ON)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_EXAMPLES)
    option(JKQtPlotter_BUILD_EXAMPLES "Build the examples" ON)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_TOOLS)
    option(JKQtPlotter_BUILD_TOOLS "Build the tools" ON)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_FORCE_NO_PRINTER_SUPPORT)
    option(JKQtPlotter_BUILD_FORCE_NO_PRINTER_SUPPORT "Build the library with printer support switched off, even if it is available" OFF)
endif()
if(NOT DEFINED JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
    option(JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS "Build the library using precompiled headers to improve compile speed" ON)
endif()
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    option(CMAKE_INSTALL_PREFIX "Install directory" ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif()


#evaluate the settings above
if (NOT CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (NOT CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "bin")
endif()

if (NOT CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR "include")
endif()

# place all DLLs and EXEs in the subdirectory output of the top level directory of the build tree
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)


if(JKQtPlotter_BUILD_DECORATE_LIBNAMES_WITH_BUILDTYPE)
    set(JKQtPlotter_LIBNAME_ADDITION "_$<CONFIG>") #"_${CMAKE_BUILD_TYPE}")
else()
    set(JKQtPlotter_LIBNAME_ADDITION )
endif()

