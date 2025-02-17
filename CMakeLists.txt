cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(clstl_test ${PROJECT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(clstl_test ${TEST_SRC})
add_definitions(-Werror -Wpedantic -Wall -Wextra)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(CLSTL_DEBUG)
  target_link_libraries(clstl_test -fsanitize=address -fsanitize=undefined
                        -fno-omit-frame-pointer)
endif()

target_link_libraries(clstl_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(clstl_test)
