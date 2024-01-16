cmake_minimum_required(VERSION 3.23)

set(libBaseName JKQTMathText)
set(lib_name ${libBaseName}${jkqtplotter_LIBNAME_VERSION_PART})

message( STATUS "-- Building ${lib_name}" )

# create the library (SHARED/STATIC is controlled by the CMake option BUILD_SHARED_LIBS
jkqtplotter_add_library(${lib_name} )

# set all default library options
jkqtplotter_setDefaultLibOptions(${lib_name} ${libBaseName} "${CMAKE_CURRENT_LIST_DIR}/private/jkqtmathtext_precomp.h")

# DEPENDENCIES: Qt
target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${lib_name} PUBLIC ${jkqtplotter_namespace}JKQTCommon${jkqtplotter_LIBNAME_VERSION_PART})

# add source files
target_sources(${lib_name} PRIVATE
    jkqtmathtext.cpp
    jkqtmathtexttools.cpp
    jkqtmathtextlabel.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
        jkqtmathtext.h
        jkqtmathtexttools.h
        jkqtmathtextlabel.h
        jkqtmathtext_imexport.h
)

add_subdirectory(nodes)
add_subdirectory(parsers)
add_subdirectory(resources)

# Installation
jkqtplotter_installlibrary_new(${lib_name} ${libBaseName} ${CMAKE_CURRENT_LIST_DIR})


