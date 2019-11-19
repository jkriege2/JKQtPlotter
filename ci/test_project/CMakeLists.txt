cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(test_project LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt
find_package(Qt5 COMPONENTS Core Widgets PrintSupport Svg Xml REQUIRED)

# JKQtPlotter
find_package(JKQTCommonSharedLib REQUIRED)
find_package(JKQTMathTextSharedLib REQUIRED)
find_package(JKQTPlotterSharedLib REQUIRED)

add_executable(test_project main.cpp)
target_link_libraries(test_project JKQTPlotterSharedLib)
