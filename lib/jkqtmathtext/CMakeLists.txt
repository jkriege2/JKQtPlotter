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

# add XITS as a ressource
if(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
  qt_add_resources(QRCSOURCES resources/xits.qrc)
  target_sources(${lib_name} PRIVATE ${QRCSOURCES})
  target_compile_definitions(${lib_name} PUBLIC JKQTMATHTEXT_COMPILED_WITH_XITS)
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/resources/xits/OFL.txt" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_XITS_LICENSE.txt" )
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/resources/xits/README.md" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_XITS_README.md" )
endif(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)

# add FIRAMATH as a ressource
if(JKQtPlotter_BUILD_INCLUDE_FIRAMATH_FONTS)
  qt_add_resources(QRCSOURCES resources/firamath.qrc)
  target_sources(${lib_name} PRIVATE ${QRCSOURCES})
  target_compile_definitions(${lib_name} PUBLIC JKQTMATHTEXT_COMPILED_WITH_FIRAMATH)
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/resources/firaMath/LICENSE" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_FIRAMATH_LICENSE" )
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/resources/firaMath/README.md" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_FIRAMATH_README.md" )
endif(JKQtPlotter_BUILD_INCLUDE_FIRAMATH_FONTS)

# Installation
jkqtplotter_installlibrary_new(${lib_name} ${libBaseName} ${CMAKE_CURRENT_LIST_DIR})


