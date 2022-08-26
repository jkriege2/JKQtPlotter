cmake_minimum_required(VERSION 3.16)

set(EXAMPLE_NAME jkqtmathtext_render)
set(EXENAME ${EXAMPLE_NAME})

message( STATUS "..   Building Tool ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES
    ${EXAMPLE_NAME}.cpp
)
set(HEADERS  )
set(RESOURCES  )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_link_libraries(${EXENAME} JKQTPExampleToolsLib)
target_include_directories(${EXENAME} PRIVATE ../../lib)
if(JKQtPlotter_BUILD_STATIC_LIBS)
    target_link_libraries(${EXENAME} JKQTMathTextLib)
elseif(JKQtPlotter_BUILD_SHARED_LIBS)
    target_link_libraries(${EXENAME} JKQTMathTextSharedLib)
endif()



# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})


