cmake_minimum_required(VERSION 3.1)


if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()
