cmake_minimum_required(VERSION 3.1)


if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

if (NOT ${QT_PRINT_SUPPORT})
    add_compile_definitions(QT_NO_PRINTER)
endif ()