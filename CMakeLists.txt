cmake_minimum_required(VERSION 3.21)
project(project_name VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build the test targets" OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*/*.cpp
)

add_library(app_lib STATIC ${APP_SOURCES})
target_include_directories(app_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_features(app_lib PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(app_lib PRIVATE /W4 /permissive-)
else()
  target_compile_options(app_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE app_lib)
target_compile_features(app PRIVATE cxx_std_20)

include(CTest)
enable_testing()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
