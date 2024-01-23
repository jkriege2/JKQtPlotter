cmake_minimum_required(VERSION 3.23)

set(TEST_NAME jkqtcommon_test)
set(EXENAME ${TEST_NAME})

message( STATUS "..   Building Unit Test ${TEST_NAME}" )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_executable(${EXENAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib)
target_link_libraries(${EXENAME} PRIVATE ${jkqtplotter_namespace}JKQTCommon${jkqtplotter_LIBNAME_VERSION_PART})
target_link_libraries(${EXENAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_sources(${EXENAME}
  PRIVATE
    ${TEST_NAME}.cpp
)

# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
add_test(NAME ${TEST_NAME} COMMAND COMMAND $<TARGET_FILE:${EXENAME}>)

#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})


