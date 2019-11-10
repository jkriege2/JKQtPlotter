cmake_minimum_required(VERSION 3.0)

set(EXAMPLE_NAME jkqtplot_test)
set(EXENAME ${EXAMPLE_NAME})

message( STATUS "..   Building Example ${EXAMPLE_NAME}" )


# Set up source files
set(SOURCES jkqtplot_test.cpp testmain.cpp TestWidgetBarcharts.cpp TestWidgetContourPlots.cpp TestWidgetEmptyPlot.cpp TestWidgetFunctionPlots.cpp TestWidgetGeometry.cpp TestWidgetGraphs.cpp TestWidgetImages.cpp TestWidgetLogGraphs.cpp TestWidgetParamScatterPlots.cpp TestWidgetPeaksPlots.cpp TestWidgetRGBImages.cpp )
#    EmfEngine/src/EmfEngine.cpp
#    EmfEngine/src/EmfPaintDevice.cpp
#    QTeXEngine/src/QTeXPaintDevice.h
#    QTeXEngine/src/QTeXPaintEngine.h
#)
set(HEADERS testmain.h TestWidgetBarcharts.h TestWidgetContourPlots.h TestWidgetEmptyPlot.h TestWidgetFunctionPlots.h TestWidgetGeometry.h TestWidgetGraphs.h TestWidgetImages.h TestWidgetLogGraphs.h TestWidgetParamScatterPlots.h TestWidgetPeaksPlots.h TestWidgetRGBImages.h)
#    jkqtpemfengineadapter.h
    
#    EmfEngine/src/EmfEngine.h
#    QTeXEngine/src/QTeXEngine.h
#)
set(RESOURCES jkqtplot_test.qrc )
set(UIS  )

add_executable(${EXENAME} WIN32 ${SOURCES}  ${HEADERS} ${RESOURCES} ${UIS})
target_include_directories(${EXENAME} PRIVATE ../../lib)
if(BUILD_STATIC_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterLib)
elseif(BUILD_SHARED_LIBS)
    target_link_libraries(${EXENAME} JKQTPlotterSharedLib)
endif()



# Installation
if(LIB_INSTALL)
    install(TARGETS ${EXENAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif(LIB_INSTALL)
#Installation of Qt DLLs on Windows
jkqtplotter_deployqt(${EXENAME})
