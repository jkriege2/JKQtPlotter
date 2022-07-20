cmake_minimum_required(VERSION 3.15)

set(libBasename JKQTCommon)
set(libIncludeSubdir jkqtcommon)

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
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpdebuggingtools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpmathtools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpalgorithms.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstringtools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtplinalgtools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpcodestructuring.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpbasicimagetools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpgeometrytools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpdrawingtools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpenhancedpainter.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtphighrestimer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpmathparser.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqttools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtparraytools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatbasics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstathistogram.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatkde.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatregression.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatpoly.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpstatgrouped.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpicons.cpp
)

set(HEADERS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtcommon_imexport.h>
  $<INSTALL_INTERFACE:jkqtcommon_imexport.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpdebuggingtools.h>
  $<INSTALL_INTERFACE:jkqtpdebuggingtools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpmathtools.h>
  $<INSTALL_INTERFACE:jkqtpmathtools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpalgorithms.h>
  $<INSTALL_INTERFACE:jkqtpalgorithms.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpstringtools.h>
  $<INSTALL_INTERFACE:jkqtpstringtools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtplinalgtools.h>
  $<INSTALL_INTERFACE:jkqtplinalgtools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpcodestructuring.h>
  $<INSTALL_INTERFACE:jkqtpcodestructuring.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpbasicimagetools.h>
  $<INSTALL_INTERFACE:jkqtpbasicimagetools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpgeometrytools.h>
  $<INSTALL_INTERFACE:jkqtpgeometrytools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpdrawingtools.h>
  $<INSTALL_INTERFACE:jkqtpdrawingtools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpenhancedpainter.h>
  $<INSTALL_INTERFACE:jkqtpenhancedpainter.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtphighrestimer.h>
  $<INSTALL_INTERFACE:jkqtphighrestimer.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpmathparser.h>
  $<INSTALL_INTERFACE:jkqtpmathparser.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqttools.h>
  $<INSTALL_INTERFACE:jkqttools.h>
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
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpicons.h>
  $<INSTALL_INTERFACE:jkqtpicons.h>
)

include(CMakePackageConfigHelpers)

function(JKQtCommon_setDefaultLibOptions TARGETNAME)
    set_property(TARGET ${TARGETNAME} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Widgets)
    if(${QT_VERSION_MAJOR} VERSION_LESS "6")
        set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD 11)
        set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD_REQUIRED TRUE)
        target_compile_features(${TARGETNAME} PUBLIC cxx_std_11)
    else()
        set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD 17)
        set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD_REQUIRED TRUE)
        target_compile_features(${TARGETNAME} PUBLIC cxx_std_17)
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
    # precomiled headers to speed up compilation
    target_precompile_headers(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtcommon_precomp.h)
    target_sources(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtcommon_precomp.h)
endfunction()


if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED ${SOURCES}  ${HEADERS} )
    JKQtCommon_setDefaultLibOptions(${libsh_name})
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_compile_definitions(${libsh_name} PUBLIC JKQTCOMMON_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTCOMMON_LIB_EXPORT_LIBRARY)
    set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake 
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC ${SOURCES} ${HEADERS})
    JKQtCommon_setDefaultLibOptions(${lib_name})
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
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

    set(JKQTP_CURRENT_TARGET_FILENAME "${libsh_name}Targets.cmake")
    configure_file(LibTarget.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Config.cmake" @ONLY)
    install(EXPORT ${libsh_name}_TARGETS
        FILE "${JKQTP_CURRENT_TARGET_FILENAME}"
        DESTINATION lib/cmake
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake"
                    DESTINATION lib/cmake  )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Config.cmake"
                                DESTINATION lib/cmake  )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
    )
    set(JKQTP_CURRENT_TARGET_FILENAME "${lib_name}Targets.cmake")
    configure_file(LibTarget.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Config.cmake" @ONLY)
    install(EXPORT ${lib_name}_TARGETS
        FILE "${JKQTP_CURRENT_TARGET_FILENAME}"
        DESTINATION lib/cmake
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake"
                    DESTINATION lib/cmake  )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Config.cmake"
                                DESTINATION lib/cmake  )

endif(JKQtPlotter_BUILD_STATIC_LIBS)

install(FILES ${HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
        COMPONENT Headers)

configure_file(${PROJECT_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt" DESTINATION doc/JKQtPlotter )
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION doc/JKQtPlotter RENAME "${lib_name}_LICENSE.txt" )

