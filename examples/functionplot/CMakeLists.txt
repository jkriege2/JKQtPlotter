cmake_minimum_required(VERSION 3.16)

set(EXAMPLE_NAME functionplot)
set(EXENAME jkqtptest_${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES ${EXAMPLE_NAME}.cpp)
set(HEADERS )
set(RESOURCES  )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib)
if(JKQtPlotter_BUILD_STATIC_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterLib)

elseif(JKQtPlotter_BUILD_SHARED_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterSharedLib)

endif()

# precomiled headers to speed up compilation
if (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
  target_precompile_headers(${EXENAME} REUSE_FROM jkqtptest_simpletest)
endif (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)


# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})
