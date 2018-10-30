cmake_minimum_required(VERSION 3.12)
project(main_executor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(main_executor COMMAND make -C ${main_executor_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})
