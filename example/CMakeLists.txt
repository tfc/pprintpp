cmake_minimum_required(VERSION 3.12)
project(pprintpp-example CXX)

find_package(pprintpp QUIET)

add_executable(${PROJECT_NAME}-example main.cpp)
target_link_libraries(${PROJECT_NAME}-example pprintpp::pprintpp)
target_compile_options(${PROJECT_NAME}-example PRIVATE -Wall -Wextra -Werror)

install (TARGETS ${PROJECT_NAME}-example RUNTIME DESTINATION bin)
