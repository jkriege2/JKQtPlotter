cmake_minimum_required(VERSION 3.23)

set(EXAMPLE_NAME paramscatterplot_customsymbol)
set(EXENAME jkqtptest_${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES paramscatterplot_customsymbol.cpp )
set(HEADERS  )
set(RESOURCES  )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_link_libraries(${EXENAME} JKQTPExampleToolsLib)
target_include_directories(${EXENAME} PRIVATE ../../lib)
if(JKQtPlotter_BUILD_STATIC_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterLib)

elseif(JKQtPlotter_BUILD_SHARED_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterSharedLib)
endif()

# precomiled headers to speed up compilation
target_precompile_headers(${EXENAME} PRIVATE ../../lib/jkqtplotter/private/jkqtplotter_precomp.h)


# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})
