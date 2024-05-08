add_library(ws2812b ws2812b.c)
target_include_directories(ws2812b INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})