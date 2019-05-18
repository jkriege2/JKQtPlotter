cmake_minimum_required(VERSION 3.1)
# common definitions
include(../../common_include.cmake)


IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
    SET(LIB_NAME libjkqtmathtext_static) 
ELSE()
    SET(LIB_NAME libjkqtmathtext_static_debug) 
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)


# configure compiler
include(${CMAKE_CURRENT_SOURCE_DIR}/../../common_compilersettings.cmake)




# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Gui PrintSupport CONFIG REQUIRED)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)


# All build libraries are moved to this directory
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/jkqtcommon/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/jkqtmathtext/)

# define output library and properties
ADD_LIBRARY(${LIB_NAME} ${SOURCES} ${HEADERS})
INSTALL(TARGETS ${LIB_NAME} DESTINATION bin)

# define target
target_link_libraries(${LIB_NAME} Qt5::Core Qt5::Widgets Qt5::PrintSupport)
#target_compile_features(${LIB_NAME} PUBLIC cxx_std_11)
target_include_directories(${LIB_NAME} PUBLIC "../../lib/jkqtmathtext/")
#export(TARGETS ${LIB_NAME})
