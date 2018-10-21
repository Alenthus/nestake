cmake_minimum_required(VERSION 3.12)

add_executable(TestCPU cpu_test.cpp)
target_link_libraries(TestCPU cpu gtest_main)
gtest_add_tests(TARGET TestCPU)