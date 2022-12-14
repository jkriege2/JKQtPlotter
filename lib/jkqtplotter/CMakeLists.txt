cmake_minimum_required(VERSION 3.15)

set(libBasename JKQTPlotter)
set(libIncludeSubdir jkqtplotter)

if(JKQtPlotter_BUILD_SHARED_LIBS)
    set(libsh_name ${libBasename}SharedLib)
    set(libsh_name_decorated ${libsh_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_SHARED_LIBRARY ${libsh_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${libsh_name}" )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    set(lib_name ${libBasename}Lib)
    set(lib_name_decorated ${lib_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_STATIC_LIBRARY ${lib_name_decorated} CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${lib_name}" )
endif(JKQtPlotter_BUILD_STATIC_LIBS)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Set up source files
set(SOURCES_BASE
    ${CMAKE_CURRENT_LIST_DIR}/jkqtptools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpbaseelements.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpbaseplotter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpdatastorage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbaseerrors.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbasestylingmixins.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtplotter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtplotterstyle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpkeystyle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpbaseplotterstyle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpcoordinateaxes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpcoordinateaxesstyle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpimagetools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbasestyle.cpp
    )
set(SOURCES_GRAPHS
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpscatter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtprange.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpspecialline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpbarchartbase.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpbarchart.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpboxplot.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpboxplotbase.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpboxplotstylingmixins.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpevaluatedfunctionbase.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpevaluatedfunction.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpfilledcurve.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeometric.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeoannotations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeobase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeolines.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeoshapes.cpp    
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimpulses.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpparsedfunction.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtppeakstream.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpsinglecolumnsymbols.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimageoverlays.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpcontour.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimagergb.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpviolinplot.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpviolinplotstylingmixins.cpp
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpstatisticsadaptors.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpevaluatedparametriccurve.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtplines.cpp 
    )
set(SOURCES_GUI
    ${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpcomboboxes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpenhancedspinboxes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpenhancedtableview.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpgraphsmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gui/jkvanishqtoolbar.cpp
)


set(HEADERS_BASE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtptools.h>
    $<INSTALL_INTERFACE:jkqtptools.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpbaseelements.h>
    $<INSTALL_INTERFACE:jkqtpbaseelements.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpbaseplotter.h>
    $<INSTALL_INTERFACE:jkqtpbaseplotter.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpdatastorage.h>
    $<INSTALL_INTERFACE:jkqtpdatastorage.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbase.h>
    $<INSTALL_INTERFACE:jkqtpgraphsbase.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbaseerrors.h>
    $<INSTALL_INTERFACE:jkqtpgraphsbaseerrors.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbasestylingmixins.h>
    $<INSTALL_INTERFACE:jkqtpgraphsbasestylingmixins.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtplotter.h>
    $<INSTALL_INTERFACE:jkqtplotter.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtplotterstyle.h>
    $<INSTALL_INTERFACE:jkqtplotterstyle.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpkeystyle.h>
    $<INSTALL_INTERFACE:jkqtpkeystyle.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpbaseplotterstyle.h>
    $<INSTALL_INTERFACE:jkqtpbaseplotterstyle.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpcoordinateaxes.h>
    $<INSTALL_INTERFACE:jkqtpcoordinateaxes.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpcoordinateaxesstyle.h>
    $<INSTALL_INTERFACE:jkqtpcoordinateaxesstyle.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpimagetools.h>
    $<INSTALL_INTERFACE:jkqtpimagetools.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtpgraphsbasestyle.h>
    $<INSTALL_INTERFACE:jkqtpgraphsbasestyle.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtplotter_configmacros.h>
    $<INSTALL_INTERFACE:jkqtplotter_configmacros.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtplotter_imexport.h>
    $<INSTALL_INTERFACE:jkqtplotter_imexport.h>
)
set(HEADERS_GRAPHS
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpboxplot.h>
    $<INSTALL_INTERFACE:graphs/jkqtpboxplot.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpboxplotbase.h>
    $<INSTALL_INTERFACE:graphs/jkqtpboxplotbase.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpboxplotstylingmixins.h>
    $<INSTALL_INTERFACE:graphs/jkqtpboxplotstylingmixins.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpevaluatedfunctionbase.h>
    $<INSTALL_INTERFACE:graphs/jkqtpevaluatedfunctionbase.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpevaluatedfunction.h>
    $<INSTALL_INTERFACE:graphs/jkqtpevaluatedfunction.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpfilledcurve.h>
    $<INSTALL_INTERFACE:graphs/jkqtpfilledcurve.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeometric.h>
    $<INSTALL_INTERFACE:graphs/jkqtpgeometric.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeoannotations.h>
    $<INSTALL_INTERFACE:graphs/jkqtpgeoannotations.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeobase.h>
    $<INSTALL_INTERFACE:graphs/jkqtpgeobase.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeolines.h>
    $<INSTALL_INTERFACE:graphs/jkqtpgeolines.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpgeoshapes.h>
    $<INSTALL_INTERFACE:graphs/jkqtpgeoshapes.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimage.h>
    $<INSTALL_INTERFACE:graphs/jkqtpimage.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimpulses.h>
    $<INSTALL_INTERFACE:graphs/jkqtpimpulses.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpparsedfunction.h>
    $<INSTALL_INTERFACE:graphs/jkqtpparsedfunction.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtppeakstream.h>
    $<INSTALL_INTERFACE:graphs/jkqtppeakstream.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpsinglecolumnsymbols.h>
    $<INSTALL_INTERFACE:graphs/jkqtpsinglecolumnsymbols.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimageoverlays.h>
    $<INSTALL_INTERFACE:graphs/jkqtpimageoverlays.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpcontour.h>
    $<INSTALL_INTERFACE:graphs/jkqtpcontour.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpimagergb.h>
    $<INSTALL_INTERFACE:graphs/jkqtpimagergb.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpviolinplot.h>
    $<INSTALL_INTERFACE:graphs/jkqtpviolinplot.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpviolinplotstylingmixins.h>
    $<INSTALL_INTERFACE:graphs/jkqtpviolinplotstylingmixins.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpstatisticsadaptors.h>
    $<INSTALL_INTERFACE:graphs/jkqtpstatisticsadaptors.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpscatter.h>
    $<INSTALL_INTERFACE:graphs/jkqtpscatter.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtprange.h>
    $<INSTALL_INTERFACE:graphs/jkqtprange.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpspecialline.h>
    $<INSTALL_INTERFACE:graphs/jkqtpspecialline.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpbarchartbase.h>
    $<INSTALL_INTERFACE:graphs/jkqtpbarchartbase.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpbarchart.h>
    $<INSTALL_INTERFACE:graphs/jkqtpbarchart.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtpevaluatedparametriccurve.h>
    $<INSTALL_INTERFACE:graphs/jkqtpevaluatedparametriccurve.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/graphs/jkqtplines.h>
    $<INSTALL_INTERFACE:graphs/jkqtplines.h>
)
set(HEADERS_GUI
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpcomboboxes.h>
    $<INSTALL_INTERFACE:gui/jkqtpcomboboxes.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpenhancedspinboxes.h>
    $<INSTALL_INTERFACE:gui/jkqtpenhancedspinboxes.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpenhancedtableview.h>
    $<INSTALL_INTERFACE:gui/jkqtpenhancedtableview.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gui/jkqtpgraphsmodel.h>
    $<INSTALL_INTERFACE:gui/jkqtpgraphsmodel.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/gui/jkvanishqtoolbar.h>
    $<INSTALL_INTERFACE:gui/jkvanishqtoolbar.h>
)

set(RESOURCES
  ${CMAKE_CURRENT_LIST_DIR}/resources/jkqtpbaseplotter.qrc
  ${CMAKE_CURRENT_LIST_DIR}/resources/jkqtpstyles.qrc
)

include(CMakePackageConfigHelpers)

function(JKQtPlottter_setDefaultLibOptions TARGETNAME)
    set_property(TARGET ${TARGETNAME} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Xml)
    if(JKQtPlotter_HAS_NO_PRINTER_SUPPORT)
        target_compile_definitions(${TARGETNAME} PUBLIC JKQTPLOTTER_HAS_NO_PRINTSUPPORT)
    else()
        target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::PrintSupport)
        target_compile_definitions(${TARGETNAME} PUBLIC JKQTPLOTTER_HAS_PRINTSUPPORT)
    endif()
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD ${JKQtPlotter_QT_CXX_STANDARD})
    set_property(TARGET ${TARGETNAME} PROPERTY CXX_STANDARD_REQUIRED ${JKQtPlotter_QT_CXX_STANDARD_REQUIRED})
    target_compile_features(${TARGETNAME} PUBLIC ${JKQtPlotter_QT_CXX_COMPILE_FEATURE})

    if(MINGW)
      # COMPILER-SETTINGS FOR MINGW
      target_compile_options(${TARGETNAME} PUBLIC -fexceptions)
    elseif(MSVC)
      # COMPILER-SETTINGS FOR MS VISUAL C++
      target_compile_options(${TARGETNAME} PUBLIC /EHsc)
      target_compile_definitions(${TARGETNAME} PUBLIC NOMINMAX)
      target_compile_definitions(${TARGETNAME} PRIVATE QT_NO_SIGNALS_SLOTS_KEYWORDS)
    endif()
    target_include_directories(${TARGETNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    # precomiled headers to speed up compilation
    if (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
      target_precompile_headers(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtplotter_precomp.h)
      target_sources(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtplotter_precomp.h)
    endif (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)

endfunction()


if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED
          ${SOURCES_BASE}
          ${SOURCES_GRAPHS}
          ${SOURCES_GUI}
          ${RESOURCES}
          ${HEADERS_BASE}
          ${HEADERS_GRAPHS}
          ${HEADERS_GUI}
    )
    JKQtPlottter_setDefaultLibOptions(${libsh_name})
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_link_libraries(${libsh_name} PUBLIC JKQTCommonSharedLib JKQTCommonStatisticsAndMathSharedLib JKQTMathTextSharedLib)
    target_compile_definitions(${libsh_name} PUBLIC JKQTPLOTTER_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTPLOTTER_LIB_EXPORT_LIBRARY)
    set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC ${SOURCES_BASE} ${SOURCES_GRAPHS} ${SOURCES_GUI} ${RESOURCES} ${HEADERS_BASE} ${HEADERS_GRAPHS} ${HEADERS_GUI})
    JKQtPlottter_setDefaultLibOptions(${lib_name})
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
    target_link_libraries(${lib_name} PUBLIC JKQTCommonLib JKQTCommonStatisticsAndMathLib JKQTMathTextLib)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()



# Installation
if(JKQtPlotter_BUILD_SHARED_LIBS)
    jkqtplotter_installlibrary(${libsh_name} ${libIncludeSubdir} "Shared")
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    jkqtplotter_installlibrary(${lib_name} ${libIncludeSubdir} "")
endif(JKQtPlotter_BUILD_STATIC_LIBS)

install(FILES ${HEADERS_BASE}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
		COMPONENT Headers)

install(FILES ${HEADERS_GRAPHS}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}/graphs
		COMPONENT Headers)

install(FILES ${HEADERS_GUI}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}/gui
		COMPONENT Headers)

configure_file(${PROJECT_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${libBasename}_Readme.txt @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libBasename}_Readme.txt" DESTINATION doc/JKQtPlotter )
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION doc/JKQtPlotter RENAME "${libBasename}_LICENSE.txt" )


#get_target_property(printsupp_inc Qt${QT_VERSION_MAJOR}::PrintSupport INTERFACE_INCLUDE_DIRECTORIES)
#message("Qt${QT_VERSION_MAJOR}::PrintSupport::INTERFACE_INCLUDE_DIRECTORIES = ${printsupp_inc}")
