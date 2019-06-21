cmake_minimum_required(VERSION 3.0)

set(EXAMPLE_NAME styling)
set(EXENAME jkqtptest_${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES test_styling.cpp test_styling_main.cpp)
set(HEADERS test_styling.h )
set(RESOURCES  )
set(UIS test_styling.ui )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib)
if(BUILD_STATIC_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterLib${LIBNAME_ADDITION})
elseif(BUILD_SHARED_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterSharedLib${LIBNAME_ADDITION})
endif()



# Installation
if(LIB_INSTALL)
    install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif(LIB_INSTALL)
