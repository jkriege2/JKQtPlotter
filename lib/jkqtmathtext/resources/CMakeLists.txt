cmake_minimum_required(VERSION 3.23)


# add XITS as aressource
if(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)
  target_sources(${lib_name} PRIVATE
    xits.qrc
  )
  target_compile_definitions(${lib_name} PUBLIC JKQTMATHTEXT_COMPILED_WITH_XITS)
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/xits/OFL.txt" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_XITS_LICENSE.txt" )
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/xits/README.md" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_XITS_README.md" )
endif(JKQtPlotter_BUILD_INCLUDE_XITS_FONTS)


# add FIRAMATH as aressource
if(JKQtPlotter_BUILD_INCLUDE_FIRAMATH_FONTS)
  target_sources(${lib_name} PRIVATE
    firamath.qrc
  )
  target_compile_definitions(${lib_name} PUBLIC JKQTMATHTEXT_COMPILED_WITH_FIRAMATH)
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/firaMath/LICENSE" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_FIRAMATH_LICENSE" )
  install(FILES "${CMAKE_CURRENT_LIST_DIR}/firaMath/README.md" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME "${lib_name}_FIRAMATH_README.md" )
endif(JKQtPlotter_BUILD_INCLUDE_FIRAMATH_FONTS)

