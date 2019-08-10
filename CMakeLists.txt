cmake_minimum_required(VERSION 3.0.2)

project(calic)
set(TARGET calic)
set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/)

set(CMAKE_C_FLAGS "-std=c11")
set(WARNING_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")

file (GLOB C_FILES *.c)
set (SOURCES ${C_FILES})

add_executable(${TARGET} ${SOURCES})

install(TARGETS ${TARGET} DESTINATION /usr/local/bin)

