cmake_minimum_required(VERSION 3.0)

set(libBasename JKQTPlotter)

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
set(SOURCES
    jkqtptools.cpp
    jkqtpbaseelements.cpp
    jkqtpbaseplotter.cpp
    jkqtpdatastorage.cpp
    jkqtpoverlaysbase.cpp
    jkqtpgraphsbase.cpp
    jkqtpgraphsbaseerrors.cpp
    jkqtpgraphsbasestylingmixins.cpp
    jkqtplotter.cpp
    jkqtplotterstyle.cpp
    jkqtpkeystyle.cpp
    jkqtpbaseplotterstyle.cpp
    jkqtpcoordinateaxes.cpp
    jkqtpcoordinateaxesstyle.cpp
    jkqtpimagetools.cpp
    )
set(SOURCES_GRAPHS
    graphs/jkqtpscatter.cpp
    graphs/jkqtprange.cpp
    graphs/jkqtpspecialline.cpp
    graphs/jkqtpbarchart.cpp 
    graphs/jkqtpboxplot.cpp 
    graphs/jkqtpboxplotstylingmixins.cpp 
    graphs/jkqtpevaluatedfunction.cpp 
    graphs/jkqtpfilledcurve.cpp 
    graphs/jkqtpgeometric.cpp 
    graphs/jkqtpimage.cpp 
    graphs/jkqtpimpulses.cpp 
    graphs/jkqtpparsedfunction.cpp 
    graphs/jkqtppeakstream.cpp 
    graphs/jkqtpsinglecolumnsymbols.cpp 
    graphs/jkqtpimageoverlays.cpp 
    graphs/jkqtpcontour.cpp 
    graphs/jkqtpimagergb.cpp
    graphs/jkqtpviolinplot.cpp
    graphs/jkqtpviolinplotstylingmixins.cpp
    graphs/jkqtpstatisticsadaptors.cpp 
    )
set(SOURCES_OVERLAYS
    overlays/jkqtpbasicoverlays.cpp
)
set(SOURCES_GUI
    gui/jkqtpcomboboxes.cpp
    gui/jkqtpenhancedspinboxes.cpp
    gui/jkqtpenhancedtableview.cpp
    gui/jkqtpgraphsmodel.cpp
    gui/jkvanishqtoolbar.cpp
)


set(HEADERS
    jkqtptools.h
    jkqtpbaseelements.h
    jkqtpbaseplotter.h
    jkqtpdatastorage.h
    jkqtpoverlaysbase.h
    jkqtpgraphsbase.h
    jkqtpgraphsbaseerrors.h
    jkqtpgraphsbasestylingmixins.h
    jkqtplotter.h
    jkqtplotterstyle.h
    jkqtpkeystyle.h
    jkqtpbaseplotterstyle.h
    jkqtpcoordinateaxes.h
    jkqtpcoordinateaxesstyle.h
    jkqtpimagetools.h
)
set(HEADERS_GRAPHS
    graphs/jkqtpboxplot.h
    graphs/jkqtpboxplotstylingmixins.h
    graphs/jkqtpevaluatedfunction.h
    graphs/jkqtpfilledcurve.h
    graphs/jkqtpgeometric.h
    graphs/jkqtpimage.h
    graphs/jkqtpimpulses.h
    graphs/jkqtpparsedfunction.h
    graphs/jkqtppeakstream.h
    graphs/jkqtpsinglecolumnsymbols.h
    graphs/jkqtpimageoverlays.h
    graphs/jkqtpcontour.h
    graphs/jkqtpimagergb.h
    graphs/jkqtpviolinplot.h
    graphs/jkqtpviolinplotstylingmixins.h
    graphs/jkqtpstatisticsadaptors.h
    graphs/jkqtpscatter.h
    graphs/jkqtprange.h
    graphs/jkqtpspecialline.h
    graphs/jkqtpbarchart.h
)
set(HEADERS_OVERLAY
    overlays/jkqtpbasicoverlays.h
)
set(HEADERS_GUI
    gui/jkqtpcomboboxes.h
    gui/jkqtpenhancedspinboxes.h
    gui/jkqtpenhancedtableview.h
    gui/jkqtpgraphsmodel.h
    gui/jkvanishqtoolbar.h
)

set(RESOURCES
  resources/jkqtpbaseplotter.qrc
  resources/jkqtpstyles.qrc
)


if(BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SOURCES} ${SOURCES_GRAPHS} ${SOURCES_GUI} ${SOURCES_OVERLAYS} ${HEADERS} ${HEADERS_GRAPHS} ${HEADERS_GUI} ${HEADERS_OVERLAYS} ${RESOURCES})
    set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${libsh_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg Qt5::Xml JKQTCommonSharedLib JKQTMathTextSharedLib)
    target_compile_definitions(${libsh_name} PUBLIC JKQTP_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTP_LIB_EXPORT_LIBRARY)
endif()

if(BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC   ${SOURCES} ${SOURCES_GRAPHS} ${SOURCES_GUI} ${SOURCES_OVERLAYS} ${HEADERS} ${HEADERS_GRAPHS} ${HEADERS_GUI} ${HEADERS_OVERLAYS} ${RESOURCES})
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
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif(BUILD_SHARED_LIBS)

    if(BUILD_STATIC_LIBS)
        install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif(BUILD_STATIC_LIBS)

    install(FILES ${HEADERS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter
            COMPONENT Headers)

    install(FILES ${HEADERS_GRAPHS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter/graphs
            COMPONENT Headers)

    install(FILES ${HEADERS_GUI}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter/gui
            COMPONENT Headers)

    install(FILES ${SOURCES_OVERLAYS}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jkqtplotter/overlays
            COMPONENT Headers)

endif(LIB_INSTALL)
