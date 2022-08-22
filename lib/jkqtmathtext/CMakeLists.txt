cmake_minimum_required(VERSION 3.15)

set(libBasename JKQTMathText)
set(libIncludeSubdir jkqtmathtext)

if(JKQtPlotter_BUILD_SHARED_LIBS)
    set(libsh_name ${libBasename}SharedLib)
    set(libsh_name_decorated ${libsh_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_SHARED_LIBRARY ${libsh_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${libsh_name}" )
endif(JKQtPlotter_BUILD_SHARED_LIBS)

if(JKQtPlotter_BUILD_STATIC_LIBS)
    set(lib_name ${libBasename}Lib)
    set(lib_name_decorated ${lib_name}${JKQtPlotter_LIBNAME_ADDITION})
    set(${libBasename}_STATIC_LIBRARY ${lib_name_decorated}  CACHE STRING "name of static library for ${libBasename}" FORCE)
    message( STATUS "-- Building ${lib_name}" )
endif(JKQtPlotter_BUILD_STATIC_LIBS)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Set up source files
set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/jkqtmathtext.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtmathtexttools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtmathtextlabel.cpp
)

set(HEADERS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtmathtext.h>
  $<INSTALL_INTERFACE:jkqtmathtext.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtmathtexttools.h>
  $<INSTALL_INTERFACE:jkqtmathtexttools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtmathtextlabel.h>
  $<INSTALL_INTERFACE:jkqtmathtextlabel.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/jkqtmathtext_imexport.h>
  $<INSTALL_INTERFACE:jkqtmathtext_imexport.h>
)

set(SOURCES_NODES
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtexttextnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextbracenode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextdecoratednode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextfracnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextinstructionnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextboxinstructionnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextmodifyenvironmentnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextverticallistnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtexthorizontallistnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextmatrixnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextsqrtnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextsubsupernode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextsymbolnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextnodetools.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextwhitespacenode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextnoopnode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextverbatimnode.cpp
)

set(HEADERS_NODES
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtexttextnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtexttextnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextboxinstructionnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextboxinstructionnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextmodifyenvironmentnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextmodifyenvironmentnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextbracenode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextbracenode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextdecoratednode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextdecoratednode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextfracnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextfracnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextinstructionnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextinstructionnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextlistnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextlistnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextverticallistnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextverticallistnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtexthorizontallistnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtexthorizontallistnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextmatrixnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextmatrixnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextsqrtnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextsqrtnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextsubsupernode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextsubsupernode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextsymbolnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextsymbolnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextnodetools.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextnodetools.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextwhitespacenode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextwhitespacenode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextnoopnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextnoopnode.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/nodes/jkqtmathtextverbatimnode.h>
  $<INSTALL_INTERFACE:nodes/jkqtmathtextverbatimnode.h>
)


set(SOURCES_PARSERS
  ${CMAKE_CURRENT_LIST_DIR}/parsers/jkqtmathtextparser.cpp
  ${CMAKE_CURRENT_LIST_DIR}/parsers/jkqtmathtextlatexparser.cpp
)

set(HEADERS_PARSERS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/parsers/jkqtmathtextparser.h>
  $<INSTALL_INTERFACE:parsers/jkqtmathtextparser.h>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/parsers/jkqtmathtextlatexparser.h>
  $<INSTALL_INTERFACE:parsers/jkqtmathtextlatexparser.h>
)

if(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
    set(RESOURCES
      ${CMAKE_CURRENT_LIST_DIR}/resources/xits.qrc
    )
endif(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)


include(CMakePackageConfigHelpers)

function(JKQtMathText_setDefaultLibOptions TARGETNAME)
    set_property(TARGET ${TARGETNAME} PROPERTY VERSION "${PROJECT_VERSION}")
    target_link_libraries(${TARGETNAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)
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
    endif()
    target_include_directories(${TARGETNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    if(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
        target_compile_definitions(${TARGETNAME} PRIVATE JKQTMATHTEXT_COMPILED_WITH_XITS)
    endif(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)

    # precomiled headers to speed up compilation
    if (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
      target_precompile_headers(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtmathtext_precomp.h)
      target_sources(${TARGETNAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private/jkqtmathtext_precomp.h)
    endif(JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
endfunction()

if(JKQtPlotter_BUILD_SHARED_LIBS)
    add_library(${libsh_name} SHARED  ${SOURCES} ${RESOURCES} ${HEADERS} ${SOURCES_NODES} ${HEADERS_NODES} ${SOURCES_PARSERS} ${HEADERS_PARSERS})
    JKQtMathText_setDefaultLibOptions(${libsh_name})
    set_property(TARGET ${libsh_name} PROPERTY OUTPUT_NAME  "${libsh_name_decorated}")
    target_link_libraries(${libsh_name} PUBLIC JKQTCommonSharedLib)
    target_compile_definitions(${libsh_name} PUBLIC JKQTMATHTEXT_LIB_IN_DLL)
    target_compile_definitions(${libsh_name} PRIVATE JKQTMATHTEXT_LIB_EXPORT_LIBRARY)
    set_target_properties(${libsh_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${libsh_name}Version.cmake 
                                     VERSION ${PROJECT_VERSION} 
                                     COMPATIBILITY AnyNewerVersion )    
endif()

if(JKQtPlotter_BUILD_STATIC_LIBS)
    add_library(${lib_name} STATIC ${SOURCES} ${RESOURCES} ${HEADERS} ${SOURCES_NODES} ${HEADERS_NODES} ${SOURCES_PARSERS} ${HEADERS_PARSERS})
    JKQtMathText_setDefaultLibOptions(${lib_name})
    set_property(TARGET ${lib_name} PROPERTY OUTPUT_NAME  "${lib_name_decorated}")

    target_link_libraries(${lib_name} PUBLIC JKQTCommonLib)
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
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
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
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}
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
install(FILES ${HEADERS_NODES}
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}/nodes
                COMPONENT Headers)
install(FILES ${SOURCES_PARSERS}
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${libIncludeSubdir}/parsers
                COMPONENT Headers)

configure_file(${PROJECT_SOURCE_DIR}/readme.txt.in ${CMAKE_CURRENT_BINARY_DIR}/${libBasename}_Readme.txt @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${libBasename}_Readme.txt" DESTINATION doc/JKQtPlotter )
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION doc/JKQtPlotter RENAME "${libBasename}_LICENSE.txt" )
install(FILES "${PROJECT_SOURCE_DIR}/lib/jkqtmathtext/resources/xits/OFL.txt" DESTINATION doc/JKQtPlotter RENAME "${libBasename}_XITS_LICENSE.txt" )
install(FILES "${PROJECT_SOURCE_DIR}/lib/jkqtmathtext/resources/xits/README.md" DESTINATION doc/JKQtPlotter RENAME "${libBasename}_XITS_README.md" )

