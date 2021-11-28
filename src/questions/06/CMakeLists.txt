cmake_minimum_required(VERSION 3.17)
project(nexthink)

set(CMAKE_CXX_STANDARD 20)

add_executable(question06 main.cpp Expressions.h)
add_executable(question06SharedPtr main.cpp ExpressionsSharedPtr.h)