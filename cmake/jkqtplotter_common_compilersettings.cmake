cmake_minimum_required(VERSION 3.1)

# configure compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)

if(MINGW)
  # COMPILER-SETTINGS FOR MINGW
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
elseif(MSVC)
  # COMPILER-SETTINGS FOR MS VISUAL C++
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  # To enable M_PI, M_E,...
  add_definitions(/D_USE_MATH_DEFINES)
  # To Prevent Errors with min() and max()
  add_definitions(/DNOMINMAX)
  # To fix error: C2338: va_start argument must not
  # have reference type and must not be parenthesized
  add_definitions(/D_CRT_NO_VA_START_VALIDATION)
endif()
