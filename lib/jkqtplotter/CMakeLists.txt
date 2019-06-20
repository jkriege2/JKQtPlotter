cmake_minimum_required(VERSION 3.0)

set(libBasename JKQTPlotter)

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

find_package(Qt5 5.0 REQUIRED Core Gui Widgets PrintSupport Svg Xml)

# Set up source files
set(SRCS
    jkqtptools.cpp
    jkqtpbaseelements.cpp
    jkqtpbaseplotter.cpp
    jkqtpdatastorage.cpp
    jkqtpelementsoverlay.cpp 
    jkqtpgraphsscatter.cpp
    jkqtpgraphsrange.cpp
    jkqtpgraphsspecialline.cpp
    jkqtpgraphsbarchart.cpp 
    jkqtpgraphsbase.cpp
    jkqtpgraphsbaseerrors.cpp
    jkqtpgraphsbasestylingmixins.cpp
    jkqtpgraphsboxplot.cpp 
    jkqtpgraphsboxplotstylingmixins.cpp 
    jkqtpgraphsevaluatedfunction.cpp 
    jkqtpgraphsfilledcurve.cpp 
    jkqtpgraphsgeometric.cpp 
    jkqtpgraphsimage.cpp 
    jkqtpgraphsimpulses.cpp 
    jkqtpgraphsparsedfunction.cpp 
    jkqtpgraphspeakstream.cpp 
    jkqtpgraphssinglecolumnsymbols.cpp 
    jkqtpgraphsimageoverlays.cpp 
    jkqtpgraphscontour.cpp 
    jkqtpgraphsimagergb.cpp
    jkqtpgraphsviolinplot.cpp
    jkqtpgraphsviolinplotstylingmixins.cpp
    jkqtpgraphsstatisticsadaptors.cpp 
    jkqtplotter.cpp
    jkqtplotterstyle.cpp
    jkqtpkeystyle.cpp
    jkqtpbaseplotterstyle.cpp
    jkqtpcoordinateaxes.cpp
    jkqtpcoordinateaxesstyle.cpp
    gui/jkqtpcomboboxes.cpp
    gui/jkqtpenhancedspinboxes.cpp
    gui/jkqtpenhancedtableview.cpp
    gui/jkqtpgraphsmodel.cpp
    gui/jkvanishqtoolbar.cpp
    jkqtpimagetools.cpp
)

set(HEADERS
    jkqtptools.h
    jkqtpbaseelements.h
    jkqtpbaseplotter.h
    jkqtpdatastorage.h
    jkqtpelementsoverlay.h
    jkqtpgraphsscatter.h
    jkqtpgraphsrange.h
    jkqtpgraphsspecialline.h
    jkqtpgraphsbarchart.h
    jkqtpgraphsbase.h
    jkqtpgraphsbaseerrors.h
    jkqtpgraphsbasestylingmixins.h
    jkqtpgraphsboxplot.h
    jkqtpgraphsboxplotstylingmixins.h
    jkqtpgraphsevaluatedfunction.h
    jkqtpgraphsfilledcurve.h
    jkqtpgraphsgeometric.h
    jkqtpgraphsimage.h
    jkqtpgraphsimpulses.h
    jkqtpgraphsparsedfunction.h
    jkqtpgraphspeakstream.h
    jkqtpgraphssinglecolumnsymbols.h
    jkqtpgraphsimageoverlays.h
    jkqtpgraphscontour.h
    jkqtpgraphsimagergb.h
    jkqtpgraphsviolinplot.h
    jkqtpgraphsviolinplotstylingmixins.h
    jkqtpgraphsstatisticsadaptors.h
    jkqtplotter.h
    jkqtplotterstyle.h
    jkqtpkeystyle.h
    jkqtpbaseplotterstyle.h
    jkqtpcoordinateaxes.h
    jkqtpcoordinateaxesstyle.h
    gui/jkqtpcomboboxes.h
    gui/jkqtpenhancedspinboxes.h
    gui/jkqtpenhancedtableview.h
    gui/jkqtpgraphsmodel.h
    gui/jkvanishqtoolbar.h
    jkqtpimagetools.h
)

set(RESOURCES
  resources/jkqtpbaseplotter.qrc
  resources/jkqtpstyles.qrc
)


if(BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SRCS} ${HEADERS} ${RESOURCES})
    set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${libsh_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg Qt5::Xml JKQTCommonSharedLib JKQTMathTextSharedLib)
    target_compile_definitions(${libsh_name} PRIVATE JKQTP_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTP_LIB_EXPORT_LIBRARY)
endif()

if(BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC  ${SRCS} ${HEADERS} ${RESOURCES})
    set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${lib_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg Qt5::Xml JKQTCommonLib JKQTMathTextLib)
endif()



# Installation
if(LIB_INSTALL)

    if(BUILD_SHARED_LIBS)
        install(TARGETS ${libsh_name} EXPORT ${libsh_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter
        )
    endif(BUILD_SHARED_LIBS)

    if(BUILD_STATIC_LIBS)
        install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter
        )
    endif(BUILD_STATIC_LIBS)

    install(FILES ${HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter
            COMPONENT Headers)

endif(LIB_INSTALL)
