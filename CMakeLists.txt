cmake_minimum_required(VERSION 3.28.1)
project(ws2812b_driver VERSION 0.1.0 LANGUAGES C)

enable_testing()

add_executable(ws2812b_driver ws2812b_test.c)

add_subdirectory(ws2812b)
target_link_libraries(ws2812b_driver PUBLIC ws2812b)
add_test(NAME test COMMAND ws2812b_driver)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_include_directories(ws2812b_driver PUBLIC "${PROJECT_BINARY_DIR}")