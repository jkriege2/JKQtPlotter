
# Instruct CMake to run moc+rcc+uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)

if(QT_VERSION_MAJOR LESS 5)
    message(FATAL_ERROR "Minimum supported Qt version is 5, but you are trying to compile against Qt${QT_VERSION_MAJOR}")
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Svg Xml OpenGL REQUIRED)
if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL "6")
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
endif()
if(NOT ${JKQtPlotter_BUILD_FORCE_NO_PRINTER_SUPPORT})
  find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS PrintSupport)
else()
  message(STATUS "WARNING: not searching for Qt${QT_VERSION_MAJOR}::PrintSupport, because JKQtPlotter_BUILD_FORCE_NO_PRINTER_SUPPORT=${JKQtPlotter_BUILD_FORCE_NO_PRINTER_SUPPORT}")
endif()


set(JKQtPlotter_HAS_NO_PRINTER_SUPPORT FALSE)
set(JKQTPLOTTER_PRINTSUPPORT_LIBSTRING "PrintSupport")
if(JKQtPlotter_BUILD_FORCE_NO_PRINTER_SUPPORT)
  set(JKQtPlotter_HAS_NO_PRINTER_SUPPORT TRUE)
  set(JKQTPLOTTER_PRINTSUPPORT_LIBSTRING "")
else()
  if(TARGET Qt${QT_VERSION_MAJOR}::PrintSupport)
  else()
    set(JKQtPlotter_HAS_NO_PRINTER_SUPPORT TRUE)
  set(JKQTPLOTTER_PRINTSUPPORT_LIBSTRING "")
  endif()
  if(${QT_VERSION_MAJOR} VERSION_LESS "5" )
    set(JKQtPlotter_HAS_NO_PRINTER_SUPPORT FALSE)
    set(JKQTPLOTTER_PRINTSUPPORT_LIBSTRING "PrintSupport")
  endif()
endif()

if(${QT_VERSION_MAJOR} VERSION_LESS "6")
    set(JKQtPlotter_QT_CXX_STANDARD 11)
    set(JKQtPlotter_QT_CXX_STANDARD_REQUIRED TRUE)
    set(JKQtPlotter_QT_CXX_COMPILE_FEATURE cxx_std_11)
else()
    set(JKQtPlotter_QT_CXX_STANDARD 17)
    set(JKQtPlotter_QT_CXX_STANDARD_REQUIRED TRUE)
    set(JKQtPlotter_QT_CXX_COMPILE_FEATURE cxx_std_17)
endif()

if (JKQtPlotter_ENABLED_CXX20)
    set(JKQtPlotter_QT_CXX_STANDARD 20)
    set(JKQtPlotter_QT_CXX_STANDARD_REQUIRED TRUE)
    set(JKQtPlotter_QT_CXX_COMPILE_FEATURE cxx_std_20)
endif(JKQtPlotter_ENABLED_CXX20)

set(JKQtPlotter_QT_BINDIR $<TARGET_FILE_DIR:Qt${QT_VERSION_MAJOR}::qmake>) # ${QT_DIR}../../../../bin

if (WIN32)
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(JKQtPlotter_WINDEPLOYQT_ENV_SETUP qtenv2.bat HINTS "${_qt_bin_dir}")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      find_program(JKQtPlotter_WINDEPLOYQT_EXECUTABLE NAMES windeployqt.debug.bat HINTS "${_qt_bin_dir}")
    else()
      find_program(JKQtPlotter_WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${_qt_bin_dir}")
    endif()
    if (NOT EXISTS ${JKQtPlotter_WINDEPLOYQT_EXECUTABLE})
      find_program(JKQtPlotter_WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${_qt_bin_dir}")
    endif()
    if (NOT EXISTS ${WINDEPLOYQT_EXECUTABLE})
      find_program(JKQtPlotter_WINDEPLOYQT_EXECUTABLE NAMES windeployqt.exe HINTS "${_qt_bin_dir}")
    endif()
endif(WIN32)