cmake_minimum_required(VERSION 3.23)

# add source files
target_sources(${lib_name} PRIVATE
    jkqtmathtextnode.cpp
    jkqtmathtexttextnode.cpp
    jkqtmathtextbracenode.cpp
    jkqtmathtextdecoratednode.cpp
    jkqtmathtextfracnode.cpp
    jkqtmathtextinstructionnode.cpp
    jkqtmathtextboxinstructionnode.cpp
    jkqtmathtextmodifyenvironmentnode.cpp
    jkqtmathtextverticallistnode.cpp
    jkqtmathtexthorizontallistnode.cpp
    jkqtmathtextmatrixnode.cpp
    jkqtmathtextsqrtnode.cpp
    jkqtmathtextsubsupernode.cpp
    jkqtmathtextsymbolnode.cpp
    jkqtmathtextnodetools.cpp
    jkqtmathtextwhitespacenode.cpp
    jkqtmathtextnoopnode.cpp
    jkqtmathtextverbatimnode.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
      jkqtmathtextnode.h
      jkqtmathtexttextnode.h
      jkqtmathtextboxinstructionnode.h
      jkqtmathtextmodifyenvironmentnode.h
      jkqtmathtextbracenode.h
      jkqtmathtextdecoratednode.h
      jkqtmathtextfracnode.h
      jkqtmathtextinstructionnode.h
      jkqtmathtextlistnode.h
      jkqtmathtextverticallistnode.h
      jkqtmathtexthorizontallistnode.h
      jkqtmathtextmatrixnode.h
      jkqtmathtextsqrtnode.h
      jkqtmathtextsubsupernode.h
      jkqtmathtextsymbolnode.h
      jkqtmathtextnodetools.h
      jkqtmathtextwhitespacenode.h
      jkqtmathtextnoopnode.h
      jkqtmathtextverbatimnode.h
)