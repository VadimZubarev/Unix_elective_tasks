cmake_minimum_required(VERSION 3.22)

project(my_shm_api)

add_executable(logger client.c)

target_link_libraries(logger rt)

add_executable(client logger.c)
