project(my_process_manager)

find_package(json-c CONFIG)

add_executable(${PROJECT_NAME} my_process_manager.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE json-c::json-c)
