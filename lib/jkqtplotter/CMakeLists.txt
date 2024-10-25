cmake_minimum_required(VERSION 3.23)

set(libBaseName JKQTPlotter)
set(lib_name ${libBaseName}${jkqtplotter_LIBNAME_VERSION_PART})

message( STATUS "-- Building ${lib_name}" )

# create the library (SHARED/STATIC is controlled by the CMake option BUILD_SHARED_LIBS
jkqtplotter_add_library(${lib_name} )

# set all default library options
jkqtplotter_setDefaultLibOptions(${lib_name} ${libBaseName} "${CMAKE_CURRENT_LIST_DIR}/private/jkqtplotter_precomp.h")

# DEPENDENCIES: Qt
target_link_libraries(${lib_name} PUBLIC ${jkqtplotter_namespace}JKQTCommon${jkqtplotter_LIBNAME_VERSION_PART})
target_link_libraries(${lib_name} PUBLIC ${jkqtplotter_namespace}JKQTMath${jkqtplotter_LIBNAME_VERSION_PART})
target_link_libraries(${lib_name} PUBLIC ${jkqtplotter_namespace}JKQTMathText${jkqtplotter_LIBNAME_VERSION_PART})
target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Xml)
if(JKQtPlotter_HAS_NO_PRINTER_SUPPORT)
    target_compile_definitions(${lib_name} PUBLIC JKQTPLOTTER_HAS_NO_PRINTSUPPORT)
else()
    target_link_libraries(${lib_name} PUBLIC Qt${QT_VERSION_MAJOR}::PrintSupport)
    target_compile_definitions(${lib_name} PUBLIC JKQTPLOTTER_HAS_PRINTSUPPORT)
endif()


# add source files
target_sources(${lib_name} PRIVATE
    jkqtptools.cpp
    jkqtpbaseelements.cpp
    jkqtpbaseplotter.cpp
    jkqtpdatastorage.cpp
    jkqtpgraphsbase.cpp
    jkqtpgraphsbaseerrors.cpp
    jkqtpgraphsbasestylingmixins.cpp
    jkqtplotter.cpp
    jkqtplotterstyle.cpp
    jkqtpkeystyle.cpp
    jkqtpkey.cpp
    jkqtpbaseplotterstyle.cpp
    jkqtpcoordinateaxes.cpp
    jkqtpcoordinateaxesstyle.cpp
    jkqtpimagetools.cpp
    jkqtpgraphsbasestyle.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
    jkqtptools.h
    jkqtpbaseelements.h
    jkqtpbaseplotter.h
    jkqtpdatastorage.h
    jkqtpgraphsbase.h
    jkqtpgraphsbaseerrors.h
    jkqtpgraphsbasestylingmixins.h
    jkqtplotter.h
    jkqtplotterstyle.h
    jkqtpkeystyle.h
    jkqtpkey.h
    jkqtpbaseplotterstyle.h
    jkqtpcoordinateaxes.h
    jkqtpcoordinateaxesstyle.h
    jkqtpimagetools.h
    jkqtpgraphsbasestyle.h
    jkqtplotter_configmacros.h
    jkqtplotter_imexport.h
)

add_subdirectory(graphs)
add_subdirectory(gui)

# add ressource
qt_add_resources(QRCSOURCES
	resources/jkqtpbaseplotter.qrc
	resources/jkqtpstyles.qrc
)
target_sources(${lib_name} PRIVATE ${QRCSOURCES})

# add version info
target_include_directories(${lib_name} PRIVATE ${JKQTPLOTTER_VERSION_FILE_DIR})


# Installation
jkqtplotter_installlibrary_new(${lib_name} ${libBaseName} ${CMAKE_CURRENT_LIST_DIR})


