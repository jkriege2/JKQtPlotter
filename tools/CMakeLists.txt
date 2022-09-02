cmake_minimum_required(VERSION 3.16)


message( STATUS )
message( STATUS "............................................................................." )
message( STATUS ".. BUILDING TOOLS" )
message( STATUS "............................................................................." )
if (JKQtPlotter_BUILD_WITH_PRECOMPILED_HEADERS)
  message( STATUS "..   - Precompiled Header:  ON" )
else()
  message( STATUS "..   - Precompiled Header:  OFF")
endif()

message( STATUS "............................................................................." )




# examples specific to JKQtMathText
message( STATUS ".. BUILDING TOOLS FOR JKQTMATHTEXT:" )
add_subdirectory(jkqtmathtext_render)
add_subdirectory(jkqtplotter_doc_imagegenerator)

