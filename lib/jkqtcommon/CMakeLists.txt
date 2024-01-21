cmake_minimum_required(VERSION 3.23)

set(libBaseName JKQTCommon)
set(lib_name ${libBaseName}${jkqtplotter_LIBNAME_VERSION_PART})

message( STATUS "-- Building ${lib_name}" )

# create the library (SHARED/STATIC is controlled by the CMake option BUILD_SHARED_LIBS
jkqtplotter_add_library(${lib_name} )

# set all default library options
jkqtplotter_setDefaultLibOptions(${lib_name} ${libBaseName} "${CMAKE_CURRENT_LIST_DIR}/private/jkqtcommon_precomp.h")

# DEPENDENCIES: Qt
target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Widgets)

# add source files
target_sources(${lib_name} PRIVATE
    jkqtpdebuggingtools.cpp
    jkqtpmathtools.cpp
    jkqtpstringtools.cpp
    jkqtpcodestructuring.cpp
    jkqtpbasicimagetools.cpp
    jkqtpconcurrencytools.cpp
    jkqtpcachingtools.cpp
    jkqtpgeometrytools.cpp
    jkqtpdrawingtools.cpp
    jkqtpenhancedpainter.cpp
    jkqtphighrestimer.cpp
    jkqttools.cpp
    jkqtpicons.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
    jkqtcommon_imexport.h
    jkqtpdebuggingtools.h
    jkqtpmathtools.h
    jkqtpstringtools.h
    jkqtpcodestructuring.h
    jkqtpbasicimagetools.h
    jkqtpdrawingtools.h
    jkqtpenhancedpainter.h
    jkqtphighrestimer.h
    jkqttools.h
    jkqtpicons.h
    jkqtpgeometrytools.h
    jkqtpconcurrencytools.h
    jkqtpcachingtools.h
    jkqtpexpected.h
)



# Installation
jkqtplotter_installlibrary_new(${lib_name} ${libBaseName} ${CMAKE_CURRENT_LIST_DIR})


