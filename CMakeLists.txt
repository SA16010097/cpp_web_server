cmake_minimum_required(VERSION 2.8)
project(cpp_server)

set(BIN_NAME "my_http_server")
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_definitions(-std=c++11)
aux_source_directory(src SRC_LIST)
#aux_source_directory(/data/libco/ SRC_LIST)
#aux_source_directory(/data/libco/lib SRC_LIST)

#add_library(common_libs SRC_LIST)
#target_link_libraries(common_libs )

add_executable(my_http_server ${SRC_LIST})
target_link_libraries(${BIN_NAME} /data/libco/lib/libcolib.a)
TARGET_LINK_LIBRARIES(${BIN_NAME} dl)
