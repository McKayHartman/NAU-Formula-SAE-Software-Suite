cmake_minimum_required(VERSION 3.10)

project(CHIMERA VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include slint framework
include(FetchContent)
FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    # `release/1` will auto-upgrade to the latest Slint >= 1.0.0 and < 2.0.0
    # `release/1.0` will auto-upgrade to the latest Slint >= 1.0.0 and < 1.1.0
    GIT_TAG release/1
    SOURCE_SUBDIR api/cpp
)
FetchContent_MakeAvailable(Slint)



add_executable(CHIMERA src/main.cpp)
add_executable(CHIMERA_DASH 
  src/dashboard_Controller.cpp
  src/VehicleDataRingBuffer.cpp
)

slint_target_sources(CHIMERA_DASH UI/dashboard_UI.slint)
target_link_libraries(CHIMERA_DASH PRIVATE Slint::Slint)

if (WIN32)
    add_custom_command(TARGET CHIMERA_DASH POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CHIMERA_DASH> $<TARGET_FILE_DIR:CHIMERA_DASH> COMMAND_EXPAND_LISTS)
endif()

#option(BUILD_TESTING "Building unit tests" on)


# Build control for unit tests

# if(BUILD_TESTING)
#   include(FetchContent)

#   include(FetchContent)
#   FetchContent_Declare(
#     googletest
#     URL https://github.com
#   )
#   set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#   FetchContent_MakeAvailable(googletest)

#   enable_testing()
#   add_subdirectory(tests)
# endif()