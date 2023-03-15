cmake_minimum_required(VERSION 3.15)

set(libBasename JKQTMath)
set(oldlibBasename JKQTCommonStatisticsAndMath)
set(libIncludeSubdir jkqtmath)

if(JKQtPlotter_BUILD_SHARED_LIBS)
    set(libsh_name ${libBasename}SharedLib)
    set(libsh_name_decorated ${libsh_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(oldlibsh_name ${oldlibBasename}SharedLib)
    set(oldlibsh_name_decorated ${oldlibsh_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_SHARED_LIBRARY ${libsh_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${libsh_name}" )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    set(lib_name ${libBasename}Lib)
    set(lib_name_decorated ${lib_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(oldlib_name ${oldlibBasename}Lib)
    set(oldlib_name_decorated ${oldlib_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_STATIC_LIBRARY ${lib_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${lib_name}" )
endif(JKQtPlotter_BUILD_STATIC_LIBS)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Set up source files
set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpalgorithms.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtplinalgtools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpmathparser.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtparraytools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatbasics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstathistogram.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatkde.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatregression.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatpoly.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatgrouped.cpp
)

set(HEADERS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtmath_imexport.h>
  $<INSTALL_INTERFACE:jkqtmath_imexport.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpalgorithms.h>
  $<INSTALL_INTERFACE:jkqtpalgorithms.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtplinalgtools.h>
  $<INSTALL_INTERFACE:jkqtplinalgtools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpmathparser.h>
  $<INSTALL_INTERFACE:jkqtpmathparser.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtparraytools.h>
  $<INSTALL_INTERFACE:jkqtparraytools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstatisticstools.h>
  $<INSTALL_INTERFACE:jkqtpstatisticstools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstatbasics.h>
  $<INSTALL_INTERFACE:jkqtpstatbasics.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstathistogram.h>
  $<INSTALL_INTERFACE:jkqtpstathistogram.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstatkde.h>
  $<INSTALL_INTERFACE:jkqtpstatkde.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstatregression.h>
  $<INSTALL_INTERFACE:jkqtpstatregression.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstatpoly.h>
  $<INSTALL_INTERFACE:jkqtpstatpoly.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstatgrouped.h>
  $<INSTALL_INTERFACE:jkqtpstatgrouped.h>
)

include(CMakePackageConfigHelpers)

function(JKQtCommon_setDefaultLibOptions TARGETNAME)
    set_property(TARGET ${TARGETNAME} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core )
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD ${JKQtPlotter_QT_CXX_STANDARD})
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD_REQUIRED ${JKQtPlotter_QT_CXX_STANDARD_REQUIRED})
    target_compile_features(${TARGETNAME} PUBLIC ${JKQtPlotter_QT_CXX_COMPILE_FEATURE})
    if(MINGW)
      # COMPILER-SETTINGS FOR MINGW
      target_compile_options(${TARGETNAME} PUBLIC -fexceptions)
    elseif(MSVC)
      # COMPILER-SETTINGS FOR MS VISUAL C++
      target_compile_options(${TARGETNAME} PUBLIC /EHsc)
      target_compile_definitions(${TARGETNAME} PUBLIC NOMINMAX)
    endif()
    target_include_directories(${TARGETNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    # precomiled headers to speed up compilation
    if (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
      target_precompile_headers(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtmath_precomp.h)
      target_sources(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtmath_precomp.h)
    endif(JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
endfunction()


if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED ${SOURCES}  ${HEADERS} )
    JKQtCommon_setDefaultLibOptions(${libsh_name})
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_compile_definitions(${libsh_name} PUBLIC jkqtmath_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE jkqtmath_LIB_EXPORT_LIBRARY)
    target_link_libraries(${libsh_name} PUBLIC JKQTCommonSharedLib)
    set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake 
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
    add_library(${oldlibsh_name} ALIAS ${libsh_name})
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC ${SOURCES} ${HEADERS})
    JKQtCommon_setDefaultLibOptions(${lib_name})
    target_link_libraries(${libsh_name} PUBLIC JKQTCommonLib)
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
    add_library(${oldlib_name} ALIAS ${oldlib_name})
endif()



# Installation
if(JKQtPlotter_BUILD_SHARED_LIBS)
    jkqtplotter_installlibrary(${libsh_name} ${libIncludeSubdir} "Shared")
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    jkqtplotter_installlibrary(${lib_name} ${libIncludeSubdir} "")
endif(JKQtPlotter_BUILD_STATIC_LIBS)

install(FILES ${HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
        COMPONENT Headers)

configure_file(${PROJECT_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${libBasename}_Readme.txt @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libBasename}_Readme.txt" DESTINATION doc/JKQtPlotter )
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION doc/JKQtPlotter RENAME "${libBasename}_LICENSE.txt" )

