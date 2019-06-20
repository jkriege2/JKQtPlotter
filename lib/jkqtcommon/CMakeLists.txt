cmake_minimum_required(VERSION 3.0)

set(libBasename JKQTCommon)

message( STATUS )
message( STATUS "-----------------------------------------------------------------------------" )
if(BUILD_SHARED_LIBS)
    set(libsh_name ${libBasename}SharedLib)
    set(${libsh_name}_LIBRARY ${libsh_name} PARENT_SCOPE)
    message( STATUS "-- Building ${libsh_name}" )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
    set(lib_name ${libBasename}Lib)
    set(${lib_name}_LIBRARY ${lib_name} PARENT_SCOPE)
    message( STATUS "-- Building ${lib_name}" )
endif(BUILD_STATIC_LIBS)



# Set up source files
set(SRCS
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
  jkqtp_imexport.h
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


if(BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SRCS} ${HEADERS})
    set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${libsh_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
    target_compile_definitions(${libsh_name} PRIVATE JKQTP_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTP_LIB_EXPORT_LIBRARY)
endif()

if(BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC  ${SRCS} ${HEADERS})
    set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${lib_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)
endif()



# Installation
if(LIB_INSTALL)

    if(BUILD_SHARED_LIBS)
        install(TARGETS ${libsh_name} EXPORT ${libsh_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}jkqtcommon
        )
    endif(BUILD_SHARED_LIBS)

    if(BUILD_STATIC_LIBS)
        install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}jkqtcommon
        )
    endif(BUILD_STATIC_LIBS)

    install(FILES ${HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}jkqtcommon
            COMPONENT Headers)

endif(LIB_INSTALL)
