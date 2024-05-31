Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

enable_testing()

add_executable(jaratest 
  "input/input.cxx" 
  "math/vec2.cxx"
)

target_include_directories(jaratest PRIVATE ${catch2_SOURCE_DIR}/include)
target_include_directories(jaratest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(jaratest PRIVATE jarastatic Catch2::Catch2WithMain)
target_precompile_headers(jaratest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hxx)

catch_discover_tests(jaratest)


