cmake_minimum_required(VERSION 3.23)

set(EXAMPLE_NAME jkqtplotter_doc_imagegenerator)
set(EXENAME ${EXAMPLE_NAME})

message( STATUS "..   Building Tool ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES
    ${EXAMPLE_NAME}.cpp
)
set(HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/../../examples/styling/build_test_graphs.h
)
set(RESOURCES  )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib ${CMAKE_CURRENT_LIST_DIR}/../../examples/styling/)
target_link_libraries(${EXENAME} ${jkqtplotter_namespace}JKQTPlotter${jkqtplotter_LIBNAME_VERSION_PART})




# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})


