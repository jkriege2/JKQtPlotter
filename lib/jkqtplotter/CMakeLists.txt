cmake_minimum_required(VERSION 3.0)

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
set(SOURCES
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
    jkqtpbaseplotterstyle.cpp
    jkqtpcoordinateaxes.cpp
    jkqtpcoordinateaxesstyle.cpp
    jkqtpimagetools.cpp
    )
set(SOURCES_GRAPHS
    graphs/jkqtpscatter.cpp
    graphs/jkqtprange.cpp
    graphs/jkqtpspecialline.cpp
    graphs/jkqtpbarchartbase.cpp 
    graphs/jkqtpbarchart.cpp 
    graphs/jkqtpboxplot.cpp 
    graphs/jkqtpboxplotbase.cpp 
    graphs/jkqtpboxplotstylingmixins.cpp 
    graphs/jkqtpevaluatedfunctionbase.cpp 
    graphs/jkqtpevaluatedfunction.cpp 
    graphs/jkqtpfilledcurve.cpp 
    graphs/jkqtpgeometric.cpp 
    graphs/jkqtpgeoannotations.cpp
    graphs/jkqtpgeobase.cpp
    graphs/jkqtpgeolines.cpp
    graphs/jkqtpgeoshapes.cpp    
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
    graphs/jkqtpevaluatedparametriccurve.cpp 
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
    jkqtplotter_imexport.h
)
set(HEADERS_GRAPHS
    graphs/jkqtpboxplot.h
    graphs/jkqtpboxplotbase.h
    graphs/jkqtpboxplotstylingmixins.h
    graphs/jkqtpevaluatedfunctionbase.h
    graphs/jkqtpevaluatedfunction.h
    graphs/jkqtpfilledcurve.h
    graphs/jkqtpgeometric.h
    graphs/jkqtpgeoannotations.h
    graphs/jkqtpgeobase.h
    graphs/jkqtpgeolines.h
    graphs/jkqtpgeoshapes.h
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
    graphs/jkqtpbarchartbase.h
    graphs/jkqtpbarchart.h
    graphs/jkqtpevaluatedparametriccurve.h
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

include(CMakePackageConfigHelpers)


if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SOURCES} ${SOURCES_GRAPHS} ${SOURCES_GUI} ${HEADERS} ${HEADERS_GRAPHS} ${HEADERS_GUI} ${RESOURCES})
    set_property(TARGET ${libsh_name} PROPERTY VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_link_libraries(${libsh_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg Qt5::Xml JKQTCommonSharedLib JKQTMathTextSharedLib)
    set_property(TARGET ${libsh_name} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${libsh_name} PROPERTY CXX_STANDARD_REQUIRED TRUE)
    target_compile_features(${libsh_name} PUBLIC cxx_std_11)
    target_compile_definitions(${libsh_name} PUBLIC JKQTPLOTTER_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTPLOTTER_LIB_EXPORT_LIBRARY)
    if(MINGW)
      # COMPILER-SETTINGS FOR MINGW
      target_compile_options(${libsh_name} PUBLIC -fexceptions)
    elseif(MSVC)
      # COMPILER-SETTINGS FOR MS VISUAL C++
      target_compile_options(${libsh_name} PUBLIC /EHsc)
      target_compile_definitions(${libsh_name} PUBLIC NOMINMAX)
    endif()
    target_include_directories(${libsh_name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  
    )
    set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC   ${SOURCES} ${SOURCES_GRAPHS} ${SOURCES_GUI} ${HEADERS} ${HEADERS_GRAPHS} ${HEADERS_GUI} ${RESOURCES})
    set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")
    set_property(TARGET ${lib_name} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${lib_name} PROPERTY CXX_STANDARD_REQUIRED TRUE)
    if(MINGW)
      # COMPILER-SETTINGS FOR MINGW
      target_compile_options(${lib_name} PUBLIC -fexceptions)
    elseif(MSVC)
      # COMPILER-SETTINGS FOR MS VISUAL C++
      target_compile_options(${lib_name} PUBLIC /EHsc)
      target_compile_definitions(${lib_name} PUBLIC NOMINMAX)
    endif()
    target_compile_features(${lib_name} PUBLIC cxx_std_11)
    target_include_directories(${lib_name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  
    )
    target_link_libraries(${lib_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport Qt5::Svg Qt5::Xml JKQTCommonLib JKQTMathTextLib)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()



# Installation
if(JKQtPlotter_BUILD_SHARED_LIBS)
    install(TARGETS ${libsh_name} EXPORT ${libsh_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    set(JKQTP_CURRENT_TARGET_FILENAME "${libsh_name}Targets.cmake")
    set(JKQTP_CURRENT_TARGET_SHAREDPART "Shared")
    configure_file(LibTarget.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Config.cmake" @ONLY)
    install(EXPORT ${libsh_name}_TARGETS
        FILE "${JKQTP_CURRENT_TARGET_FILENAME}"
        DESTINATION lib/cmake
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake"
                    DESTINATION lib/cmake  )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Config.cmake"
                                DESTINATION lib/cmake  )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    set(JKQTP_CURRENT_TARGET_FILENAME "${lib_name}Targets.cmake")
    set(JKQTP_CURRENT_TARGET_SHAREDPART "")
    configure_file(LibTarget.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Config.cmake" @ONLY)
    install(EXPORT ${lib_name}_TARGETS
        FILE "${JKQTP_CURRENT_TARGET_FILENAME}"
        DESTINATION lib/cmake
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake"
                    DESTINATION lib/cmake  )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Config.cmake"
                                DESTINATION lib/cmake  )
endif(JKQtPlotter_BUILD_STATIC_LIBS)

install(FILES ${HEADERS}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
		COMPONENT Headers)

install(FILES ${HEADERS_GRAPHS}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}/graphs
		COMPONENT Headers)

install(FILES ${HEADERS_GUI}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}/gui
		COMPONENT Headers)

configure_file(${CMAKE_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${lib_name}_Readme.txt" DESTINATION doc/JKQtPlotter )
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION doc/JKQtPlotter RENAME "${lib_name}_LICENSE.txt" )


