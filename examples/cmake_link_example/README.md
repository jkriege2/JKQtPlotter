# Example (JKQTPlotter): CMake Example  {#JKQTCMakeLinkExample}

This project (see [`cmake_link_example`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/cmake_link_example) demonstrates how to link against JKQTPlotter using CMake. See http://jkriege2.github.io/JKQtPlotter/page_buildinstructions.html for details on how to build JKQTPlotter with CMake

This example uses very simple code, which simply displays a plotter and shows some data. The important part of this example is the ´CMakeLists.txt`-file:
```
	# set minimum required CMake-Version
	cmake_minimum_required(VERSION 3.10)

	# set Project name
	set(EXAMPLE_NAME simpletest)
	set(EXENAME jkqtptest_${EXAMPLE_NAME})
	project(${EXAMPLE_NAME} LANGUAGES CXX)

	# some basic configurations
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_CXX_STANDARD 11)
	#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

	# Configure project for usage of Qt5
	find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGl REQUIRED)

	# include JKQTPlotter
	find_package(JKQTCommonLib REQUIRED)
	find_package(JKQTMathTextLib REQUIRED)
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
	# ... link against Qt5 and JKQTPlotterLib
	#    (you could use JKQTPlotterSharedLib if you don't want to link againast the 
	#     static version, but against the shared/DLL version).
	target_link_libraries(${EXENAME} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::PrintSupport Qt5::Svg Qt5::Xml JKQTPlotterLib)

	# Installation
	install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

	#Installation of Qt DLLs on Windows
	include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/jkqtplotter_deployqt.cmake)
	jkqtplotter_deployqt(${EXENAME})

```

To build this example, you first need to make a subdirectory `build` and then call CMake form that subdirectory:
```.sh
    $ mkdir build
    $ cd build
    $ cmake .. -G "<GENERATOR>" "-DCMAKE_PREFIX_PATH=<path_to_your_qt_sources> -DCMAKE_MODULE_PATH=<path_to_lib/cmake_dir_of_JKQTPLOTTER>"
```
The you can use the generated makefiles (e.g. load them in an editor, or build them jsing `make`). In the last line above, you need to specify two directories:
  - `<path_to_your_qt_sources>` points to you Qt installation
  - `<path_to_lib/cmake_dir_of_JKQTPLOTTER>` points to the directory containing the `XYZ.cmake`-files from the JKQTPlotter build. Typically this is `<JKQTPLOTTER_INSTALL_DIR>/lib/cmake`, where `<JKQTPLOTTER_INSTALL_DIR>` is the directory into which you installed JKQTPlotter.