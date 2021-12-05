cmake_minimum_required(VERSION 3.17)
project(google_test)

set(CMAKE_CXX_STANDARD 20)

include_directories(/opt/homebrew/include /opt/homebrew)
include_directories(../src/)
link_directories(/opt/homebrew/lib)

add_executable(Google_Tests_run questions/03/SyncQueueMvCtorTest.cpp)
target_link_libraries(Google_Tests_run)
target_link_libraries(Google_Tests_run gtest gtest_main)
