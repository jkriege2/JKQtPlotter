cmake_minimum_required(VERSION 3.23)

set(libBaseName JKQTFastPlotter)
set(lib_name ${libBaseName}${jkqtplotter_LIBNAME_VERSION_PART})

message( STATUS "-- Building ${lib_name}" )
message(WARNING "-- You are building the sub-library JKQTFastPlotter, which is DEPRECTAED and may be removed in future!")

# create the library (SHARED/STATIC is controlled by the CMake option BUILD_SHARED_LIBS
jkqtplotter_add_library(${lib_name} )

# set all default library options
jkqtplotter_setDefaultLibOptions(${lib_name} ${libBaseName} "")

# DEPENDENCIES: Qt
target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::Widgets)
if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL "6")
    target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()
target_link_libraries(${lib_name} PUBLIC ${jkqtplotter_namespace}JKQTCommon${jkqtplotter_LIBNAME_VERSION_PART})

# add source files
target_sources(${lib_name} PRIVATE
    jkqtfastplotter.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
    jkqtfastplotter_imexport.h
    jkqtfastplotter.h
)



# Installation
jkqtplotter_installlibrary_new(${lib_name} ${libBaseName} ${CMAKE_CURRENT_LIST_DIR})
