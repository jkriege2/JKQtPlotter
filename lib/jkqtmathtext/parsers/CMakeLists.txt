cmake_minimum_required(VERSION 3.23)

# add source files
target_sources(${lib_name} PRIVATE
  jkqtmathtextparser.cpp
  jkqtmathtextlatexparser.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
      jkqtmathtextparser.h
      jkqtmathtextlatexparser.h
)