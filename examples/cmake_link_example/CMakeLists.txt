# set minimum required CMake-Version
cmake_minimum_required(VERSION 3.20)

# set Project name
set(EXAMPLE_NAME simpletest)
set(EXENAME jkqtptest_${EXAMPLE_NAME})
project(${EXAMPLE_NAME} LANGUAGES CXX)

# some basic configurations
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Configure project for usage of Qt5/Qt6
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGl REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL REQUIRED)


# include JKQTPlotter
find_package(JKQTPlotterLib REQUIRED)

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
add_executable(${EXENAME} WIN32 simpletest.cpp)
# ... link against Qt5/6 and JKQTPlotterLib
#    (you could use JKQTPlotterSharedLib if you don't want to link againast the 
#     static version, but against the shared/DLL version).
target_link_libraries(${EXENAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui ${QT_PRINT_SUPPORT_STRING} Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Xml JKQTPlotterLib)

# Installation
install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR})

#Installation of Qt DLLs on Windows
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/jkqtplotter_deployqt.cmake)
jkqtplotter_deployqt(${EXENAME})
