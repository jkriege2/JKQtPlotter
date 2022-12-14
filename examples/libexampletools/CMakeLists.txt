cmake_minimum_required(VERSION 3.15)

set(libBasename JKQTPExampleTools)

set(lib_name ${libBasename}Lib)
message( STATUS "-- Building ${lib_name}" )

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Set up source files
set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpexampleapplication.cpp
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpappsettingcontroller.cpp
)

set(HEADERS
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpexampleapplication.h
  ${CMAKE_CURRENT_LIST_DIR}/jkqtpappsettingcontroller.h
)


include(CMakePackageConfigHelpers)

function(jkqtpexampletools_setDefaultLibOptions TARGETNAME)
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
      target_compile_definitions(${TARGETNAME} PRIVATE QT_NO_SIGNALS_SLOTS_KEYWORDS)
    endif()
    target_include_directories(${TARGETNAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../
        ${CMAKE_CURRENT_LIST_DIR}
    )
endfunction()


add_library(${lib_name} STATIC ${SOURCES} ${RESOURCES} ${HEADERS})
jkqtpexampletools_setDefaultLibOptions(${lib_name})
if(JKQtPlotter_BUILD_STATIC_LIBS)
    target_link_libraries(${lib_name} PUBLIC JKQTPlotterLib)
elseif(JKQtPlotter_BUILD_SHARED_LIBS)
    target_link_libraries(${lib_name} PUBLIC JKQTPlotterSharedLib)
endif()
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${lib_name}Version.cmake
                                 VERSION ${PROJECT_VERSION} 
                                 COMPATIBILITY AnyNewerVersion )    


