# Example (JKQTPlotter): CMake Example  {#JKQTCMakeLinkExample}

This project (see [`cmake_link_example`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/cmake_link_example) demonstrates how to link against JKQTPlotter using CMake. See http://jkriege2.github.io/JKQtPlotter/page_buildinstructions.html for details on how to build JKQTPlotter with CMake

This example uses very simple code, which simply displays a plotter and shows some data. The important part of this example is the ´CMakeLists.txt`-file:
```
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
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::PrintSupport Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Xml)
# ... link against JKQTPlotter: As the Targets contain the Qt-Version-Number in their names, we can
#     link against 'JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR}' and it works
#     for Qt5 AND Qt6 ...
#     if you have a speific Qt-Version, you can also write e.g. 'JKQTPlotter6::JKQTPlotter6'
target_link_libraries(${PROJECT_NAME} JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR})


# Installation
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

```

The important steps here are
```
# find JKQTPlotter
find_package(JKQTPlotter${QT_VERSION_MAJOR} REQUIRED)

# ...

# link against JKQtPlotter:
target_link_libraries(${EXENAME} JKQTPlotter${QT_VERSION_MAJOR}::JKQTPlotter${QT_VERSION_MAJOR})
```

We are using `${QT_VERSION_MAJOR}` here, as this file is supposed to work with both Qt5 AND Qt6. If you only use say Qt6, you can also write:
```
find_package(JKQTPlotter6 REQUIRED)
target_link_libraries(${EXENAME} JKQTPlotter6::JKQTPlotter6)
```



To build this example, you first need to make a subdirectory `build` and then call CMake form that subdirectory:
```.sh
    $ mkdir build
    $ cd build
    $ cmake .. -G "<GENERATOR>" "-DCMAKE_PREFIX_PATH=<path_to_your_qt_sources>;<path_to_JKQTPlotter_installdir>"
```
The you can use the generated makefiles (e.g. load them in an editor, or build them jsing `make`). In the last line above, you need to specify two directories:
  - `<path_to_your_qt_sources>` points to you Qt installation
  - `<path_to_JKQTPlotter_installdir>` points to the directory containing the JKQTPlotter build. Typically this is will conatin a subdirectory`<JKQTPLOTTER_INSTALL_DIR>/lib/cmake/JKQtPlottr6` with all necessary CMake-scripts. Here `<JKQTPLOTTER_INSTALL_DIR>` is the directory into which you installed JKQTPlotter.
  
