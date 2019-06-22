cmake_minimum_required(VERSION 3.0)

set(libBasename JKQTCommon)
set(libIncludeSubdir jkqtcommon)

if(BUILD_SHARED_LIBS)
    set(libsh_name ${libBasename}SharedLib)
    set(libsh_name_decorated ${libsh_name}${LIBNAME_ADDITION})
    set(${libBasename}_SHARED_LIBRARY ${libsh_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${libsh_name}" )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    set(lib_name ${libBasename}Lib)
    set(lib_name_decorated ${lib_name}${LIBNAME_ADDITION})
    set(${libBasename}_STATIC_LIBRARY ${lib_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${lib_name}" )
endif(BUILD_STATIC_LIBS)



# Set up source files
set(SOURCES
  jkqtpdebuggingtools.cpp
  jkqtpmathtools.cpp
  jkqtpalgorithms.cpp
  jkqtpstringtools.cpp
  jkqtplinalgtools.cpp
  jkqtpcodestructuring.cpp
  jkqtpbasicimagetools.cpp
  jkqtpdrawingtools.cpp
  jkqtpenhancedpainter.cpp
  jkqtphighrestimer.cpp
  jkqtpmathparser.cpp
  jkqttools.cpp
  jkqtparraytools.cpp
  jkqtpstatbasics.cpp
  jkqtpstathistogram.cpp
  jkqtpstatkde.cpp
  jkqtpstatregression.cpp
  jkqtpstatpoly.cpp
  jkqtpstatgrouped.cpp
)

set(HEADERS
  jkqtcommon_imexport.h
  jkqtpdebuggingtools.h
  jkqtpmathtools.h
  jkqtpalgorithms.h
  jkqtpstringtools.h
  jkqtplinalgtools.h
  jkqtpcodestructuring.h
  jkqtpbasicimagetools.h
  jkqtpdrawingtools.h
  jkqtpenhancedpainter.h
  jkqtphighrestimer.h
  jkqtpmathparser.h
  jkqttools.h
  jkqtparraytools.h
  jkqtpstatisticstools.h
  jkqtpstatbasics.h
  jkqtpstathistogram.h
  jkqtpstatkde.h
  jkqtpstatregression.h
  jkqtpstatpoly.h
  jkqtpstatgrouped.h
)

include(CMakePackageConfigHelpers)


if(BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SOURCES} ${HEADERS})
    set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_link_libraries(${libsh_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
    target_compile_definitions(${libsh_name} PUBLIC JKQTCOMMON_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTCOMMON_LIB_EXPORT_LIBRARY)
    set_property(TARGET ${libsh_name} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake 
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC  ${SOURCES} ${HEADERS})
    set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
    target_link_libraries(${lib_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()



# Installation
if(LIB_INSTALL)

    if(BUILD_SHARED_LIBS)
        install(TARGETS ${libsh_name} EXPORT ${libsh_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
        )
        install(EXPORT ${libsh_name}_TARGETS
            FILE ${libsh_name}Config.cmake 
            DESTINATION lib/cmake
        )
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake" DESTINATION lib/cmake  )
    endif(BUILD_SHARED_LIBS)

    if(BUILD_STATIC_LIBS)
        install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
        )
        install(EXPORT ${lib_name}_TARGETS
            FILE ${lib_name}Config.cmake 
            DESTINATION lib/cmake
        )
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake" DESTINATION lib/cmake  )
    endif(BUILD_STATIC_LIBS)

    install(FILES ${HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
            COMPONENT Headers)

    configure_file(${CMAKE_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt" DESTINATION doc/ )
    install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION doc/ RENAME "${lib_name}_LICENSE.txt" )
endif(LIB_INSTALL)
