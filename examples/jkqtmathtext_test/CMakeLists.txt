cmake_minimum_required(VERSION 3.23)

set(EXAMPLE_NAME jkqtmathtext_test)
set(EXENAME jkqtptest_${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES
    ${EXAMPLE_NAME}.cpp
    testform.cpp
)
set(HEADERS testform.h )
set(RESOURCES asana.qrc )
set(UIS testform.ui )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib)
target_link_libraries(${EXENAME} ${jkqtplotter_namespace}JKQTMathText${jkqtplotter_LIBNAME_VERSION_PART})



# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})
