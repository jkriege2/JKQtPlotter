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


# include JKQTPlotter via FetchContent-API:
# ... first load the FetchContent-API:
include(FetchContent) # once in the project to include the module
# ... now declare JKQTPlotter5/6
FetchContent_Declare(JKQTPlotter${QT_VERSION_MAJOR}
                     GIT_REPOSITORY https://github.com/jkriege2/JKQtPlotter.git
                     # GIT_TAG        v5.0.0
                     )
# ... finally make JKQTPlotter5/6 available
FetchContent_MakeAvailable(JKQTPlotter${QT_VERSION_MAJOR})

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
