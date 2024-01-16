cmake_minimum_required(VERSION 3.23)

# add source files
target_sources(${lib_name} PRIVATE
    jkqtpcomboboxes.cpp
    jkqtpenhancedspinboxes.cpp
    jkqtpenhancedtableview.cpp
    jkqtpgraphsmodel.cpp
    jkvanishqtoolbar.cpp
)
# ... and add headers
target_sources(${lib_name} PUBLIC FILE_SET HEADERS TYPE HEADERS
    FILES
    jkqtpcomboboxes.h
    jkqtpenhancedspinboxes.h
    jkqtpenhancedtableview.h
    jkqtpgraphsmodel.h
    jkvanishqtoolbar.h
)