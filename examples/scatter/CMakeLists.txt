cmake_minimum_required(VERSION 3.23)

set(EXAMPLE_NAME scatter)
set(EXENAME jkqtptest_${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES scatter.cpp )
set(HEADERS  )
set(RESOURCES  )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_link_libraries(${EXENAME} JKQTPExampleToolsLib)
target_include_directories(${EXENAME} PRIVATE ../../lib)
target_link_libraries(${EXENAME} ${jkqtplotter_namespace}JKQTPlotter${jkqtplotter_LIBNAME_VERSION_PART})

# precomiled headers to speed up compilation
target_precompile_headers(${EXENAME} PRIVATE ../../lib/jkqtplotter/private/jkqtplotter_precomp.h)


# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})
