# set minimum required CMake-Version
cmake_minimum_required(VERSION 3.23)

# set Project name
project(simpletest_cmake LANGUAGES CXX)

# some basic configurations
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure project for usage of Qt5/Qt6
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGl REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL REQUIRED)


message( STATUS )
message( STATUS "=============================================================================" )
message( STATUS "== JKQTPlotterLib:simpletest_cmake Build Information                       ==" )
message( STATUS "=============================================================================" )
message( STATUS "Version:               ${PROJECT_VERSION}")
message( STATUS "Build type:            ${CMAKE_BUILD_TYPE}")
message( STATUS "Build types:           ${CMAKE_CONFIGURATION_TYPES}")
message( STATUS "Using CMake:           ${CMAKE_VERSION}")
message( STATUS "Using CMake Generator: ${CMAKE_GENERATOR}")
message( STATUS "Using compiler:        ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, platform: ${CMAKE_CXX_PLATFORM_ID}" )
message( STATUS "LIB-Architecture:      CXX: ${CMAKE_CXX_LIBRARY_ARCHITECTURE} / BASE: ${CMAKE_LIBRARY_ARCHITECTURE}" )
message( STATUS "System Name:           ${CMAKE_SYSTEM_NAME}" )
message( STATUS "Processor Name:        ${CMAKE_SYSTEM_PROCESSOR}" )
message( STATUS "Using Qt:              ${QT_VERSION}" )
message( STATUS "                       [at ${QT_DIR}]" )


# include JKQTPlotter
find_package(JKQTPlotter${QT_VERSION_MAJOR} REQUIRED)

# For Visual Studio, we need to set some additional compiler options
if(MSVC)
    add_compile_options(/EHsc)
    # To enable M_PI, M_E,...
    add_definitions(/D_USE_MATH_DEFINES)
    # To Prevent Errors with min() and max()
    add_definitions(/DNOMINMAX)
    # To fix error: C2338: va_start argument must not
    # have reference type and must not be parenthesized
    add_definitions(/D_CRT_NO_VA_START_VALIDATION)
endif()

# add the example executable 
add_executable(${PROJECT_NAME} WIN32 simpletest.cpp)
# ... link against Qt5/6 and JKQTPlotterLib
#    (you could use JKQTPlotterSharedLib if you don't want to link againast the 
#     static version, but against the shared/DLL version).
target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::PrintSupport Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Xml)
# ... link against JKQTPlotter: As the Targets contain the Qt-Version-Number in their names, we can
#     link against 'JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR}' and it works
#     for Qt5 AND Qt6 ...
#     if you have a speific Qt-Version, you can also write e.g. 'JKQTPlotter6::JKQTPlotter6'
target_link_libraries(${PROJECT_NAME} PUBLIC JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR})


# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
