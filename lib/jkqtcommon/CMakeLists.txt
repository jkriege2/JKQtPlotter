cmake_minimum_required(VERSION 3.1)
# common definitions
include(../../common_include.cmake)

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/jkqtptools.cpp
   PARENT_SCOPE
)

set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/jkqtptools.h
   ${CMAKE_CURRENT_SOURCE_DIR}/jkqtp_imexport.h
   PARENT_SCOPE
)