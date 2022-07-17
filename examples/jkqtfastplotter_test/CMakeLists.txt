cmake_minimum_required(VERSION 3.16)

set(EXAMPLE_NAME jkqtfastplotter_test)
set(EXENAME jkqtptest_${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES
    jkqtfastplotter_test.cpp
    jkqtfastplotter_test_testmain.cpp
)
set(HEADERS jkqtfastplotter_test_testmain.h )
set(RESOURCES jkqtfastplotter_test.qrc  )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib)
if(JKQtPlotter_BUILD_STATIC_LIBS)
    target_link_libraries(${EXENAME} JKQTFastPlotterLib)
elseif(JKQtPlotter_BUILD_SHARED_LIBS)
    target_link_libraries(${EXENAME} JKQTFastPlotterSharedLib)
endif()



# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})
