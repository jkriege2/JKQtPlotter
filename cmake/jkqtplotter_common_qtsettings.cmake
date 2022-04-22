
# Instruct CMake to run moc+rcc+uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets PrintSupport Svg Xml OpenGL REQUIRED)
if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL "6")
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
endif()

# configure compiler
if(${QT_VERSION_MAJOR} VERSION_LESS "6" )
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED on)
else()
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED on)
endif()
