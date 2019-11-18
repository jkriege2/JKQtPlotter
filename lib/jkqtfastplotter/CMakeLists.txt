cmake_minimum_required(VERSION 3.0)

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
  jkqtfastplotter.cpp
)

set(HEADERS
  jkqtfastplotter.h
  jkqtfastplotter_imexport.h
)


include(CMakePackageConfigHelpers)


if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SOURCES} ${HEADERS} ${RESOURCES})
	target_compile_features(${libsh_name} PUBLIC cxx_std_11)
    set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_link_libraries(${libsh_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::OpenGL JKQTCommonSharedLib)
    target_compile_definitions(${libsh_name} PUBLIC JKQTFASTPLOTTER_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTFASTPLOTTER_LIB_EXPORT_LIBRARY)
	target_include_directories(${libsh_name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  
    )
    set_property(TARGET ${libsh_name} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake 
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC  ${SOURCES} ${HEADERS} ${RESOURCES})
	target_compile_features(${lib_name} PUBLIC cxx_std_11)
    set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
    target_link_libraries(${lib_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::OpenGL JKQTCommonLib)
	target_include_directories(${lib_name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  
    )
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()



# Installation
if(JKQtPlotter_BUILD_SHARED_LIBS)
	install(TARGETS ${libsh_name} EXPORT ${libsh_name}_TARGETS
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
	)
	install(EXPORT ${libsh_name}_TARGETS
		FILE ${libsh_name}Config.cmake 
		DESTINATION lib/cmake/${libsh_name}
	)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake" 
	        DESTINATION lib/cmake/${libsh_name}  )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
	install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
	)
	install(EXPORT ${lib_name}_TARGETS
		FILE ${lib_name}Config.cmake 
		DESTINATION lib/cmake/${lib_name}
	)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake" 
	        DESTINATION lib/cmake/${lib_name}  )
endif(JKQtPlotter_BUILD_STATIC_LIBS)

install(FILES ${HEADERS}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
		COMPONENT Headers)

configure_file(${CMAKE_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt" DESTINATION doc/ )
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION doc/ RENAME "${lib_name}_LICENSE.txt" )

