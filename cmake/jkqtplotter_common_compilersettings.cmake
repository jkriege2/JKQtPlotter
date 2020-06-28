cmake_minimum_required(VERSION 3.1)

# configure compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()
