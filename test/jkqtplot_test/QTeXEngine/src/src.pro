# qmake project file for building the QTeXEngine libraries

include( ../config.pri )

TARGET  	 = QTeXEngine
TEMPLATE     = lib

MOC_DIR      = ../tmp
OBJECTS_DIR  = ../tmp
DESTDIR      = ../

contains(CONFIG, QTeXEngineDll) {
    CONFIG  += dll
    DEFINES += QTEXENGINE_DLL QTEXENGINE_DLL_BUILD
} else {
    CONFIG  += staticlib
}

HEADERS  = QTeXEngine.h
SOURCES += QTeXPaintEngine.cpp
SOURCES += QTeXPaintDevice.cpp
