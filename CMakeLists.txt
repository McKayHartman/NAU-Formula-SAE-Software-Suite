cmake_minimum_required(VERSION 3.10)

project(CHIMERA VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CHIMERA main.cpp)



option(BUILD_TESTING "Building unit tests" on)


# Build control for unit tests

if(BUILD_TESTING)
  include(FetchContent)

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  add_subdirectory(tests)
endif()