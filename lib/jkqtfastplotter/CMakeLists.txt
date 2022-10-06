cmake_minimum_required(VERSION 3.15)

set(libBasename JKQTFastPlotter)
set(libIncludeSubdir jkqtfastplotter)

if(JKQtPlotter_BUILD_SHARED_LIBS)
    set(libsh_name ${libBasename}SharedLib)
    set(libsh_name_decorated ${libsh_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_SHARED_LIBRARY ${libsh_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${libsh_name}" )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    set(lib_name ${libBasename}Lib)
    set(lib_name_decorated ${lib_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_STATIC_LIBRARY ${lib_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${lib_name}" )
endif(JKQtPlotter_BUILD_STATIC_LIBS)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Set up source files
set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/jkqtfastplotter.cpp
)

set(HEADERS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtfastplotter.h>
  $<INSTALL_INTERFACE:jkqtfastplotter.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtfastplotter_imexport.h>
  $<INSTALL_INTERFACE:jkqtfastplotter_imexport.h>
)


include(CMakePackageConfigHelpers)

function(JKQtFastPlotter_setDefaultLibOptions TARGETNAME)
    set_property(TARGET ${TARGETNAME} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::Widgets)
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD ${JKQtPlotter_QT_CXX_STANDARD})
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD_REQUIRED ${JKQtPlotter_QT_CXX_STANDARD_REQUIRED})
    target_compile_features(${TARGETNAME} PUBLIC ${JKQtPlotter_QT_CXX_COMPILE_FEATURE})
    if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL "6")
        target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
    endif()

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
endfunction()

if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED ${SOURCES} ${RESOURCES} ${HEADERS})
    JKQtFastPlotter_setDefaultLibOptions(${libsh_name})
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_link_libraries(${libsh_name} PUBLIC JKQTCommonSharedLib)
    target_compile_definitions(${libsh_name} PUBLIC JKQTFASTPLOTTER_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY)
    set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake 
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC ${SOURCES} ${RESOURCES} ${HEADERS})
    JKQtFastPlotter_setDefaultLibOptions(${lib_name})
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
    target_link_libraries(${lib_name} PUBLIC  JKQTCommonLib)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
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

