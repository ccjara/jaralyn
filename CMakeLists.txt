cmake_minimum_required(VERSION 3.16)
project(jaralyn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM x64)
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<$<CONFIG:Debug>:/bin>$<$<CONFIG:Release>:/rel>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<$<CONFIG:Debug>:/bin>$<$<CONFIG:Release>:/rel>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<$<CONFIG:Debug>:/bin>$<$<CONFIG:Release>:/rel>")

set(LUA_INCLUDE_DIR "D:\\dev\\lua\\include")
set(LUA_LIBRARIES "D:\\dev\\lua\\lib\\debug\\lua.lib")
set(SDL2_DIR "D:\\dev\\SDL2")
set(CMAKE_PREFIX_PATH "D:\\dev\\SDL2")
set(GLEW_ROOT "D:\\dev\\GLEW")

find_package(Lua REQUIRED)
find_package(SDL2 2.0.12 REQUIRED COMPONENTS SDL2::SDL2)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${LUA_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/deps")

add_subdirectory(src)
add_subdirectory(test)

# https://chromium.googlesource.com/external/github.com/google/crc32c/+/refs/tags/1.0.5/CMakeLists.txt

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # the highest warning level for VS
  set(CMAKE_CXX_WARNING_LEVEL 4)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
  # no exceptions
  string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  # no RTTI
  string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # -Wall for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
  # -Wextra for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
  # -Werror for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Werror")
  # -Wpedantic for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wpedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wpedantic")
  # no exceptions
  string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
  # no RTTI
  string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
