cmake_minimum_required(VERSION 3.23)

set(libBaseName JKQTMath)
set(lib_name ${libBaseName}${jkqtplotter_LIBNAME_VERSION_PART})

message( STATUS "-- Building ${lib_name}" )

# create the library (SHARED/STATIC is controlled by the CMake option BUILD_SHARED_LIBS
jkqtplotter_add_library(${lib_name} )

# set all default library options
jkqtplotter_setDefaultLibOptions(${lib_name} ${libBaseName} "${CMAKE_CURRENT_LIST_DIR}/private/jkqtmath_precomp.h")

# DEPENDENCIES: Qt
target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${lib_name} PUBLIC ${jkqtplotter_namespace}JKQTCommon${jkqtplotter_LIBNAME_VERSION_PART})

# add source files
target_sources(${lib_name} PRIVATE
    jkqtpalgorithms.cpp
    jkqtplinalgtools.cpp
    jkqtpmathparser.cpp
    jkqtparraytools.cpp
    jkqtpstatbasics.cpp
    jkqtpstathistogram.cpp
    jkqtpstatkde.cpp
    jkqtpstatregression.cpp
    jkqtpstatpoly.cpp
    jkqtpstatgrouped.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
        jkqtmath_imexport.h
        jkqtpalgorithms.h
        jkqtplinalgtools.h
        jkqtpmathparser.h
        jkqtparraytools.h
        jkqtpstatisticstools.h
        jkqtpstatbasics.h
        jkqtpstathistogram.h
        jkqtpstatkde.h
        jkqtpstatregression.h
        jkqtpstatpoly.h
        jkqtpstatgrouped.h

)

# Installation
jkqtplotter_installlibrary_new(${lib_name} ${libBaseName} ${CMAKE_CURRENT_LIST_DIR})


