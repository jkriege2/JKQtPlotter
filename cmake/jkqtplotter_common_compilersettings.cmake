cmake_minimum_required(VERSION 3.23)


if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()


math(EXPR JKQtPlotter_BIT_DEPTH ${CMAKE_SIZEOF_VOID_P}*8)

find_package( CImg QUIET )
find_package( OpenCV QUIET )
