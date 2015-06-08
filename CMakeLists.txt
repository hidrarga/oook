cmake_minimum_required(VERSION 2.6)
project(opds)

add_executable(opds main.cpp)

install(TARGETS opds RUNTIME DESTINATION bin)
