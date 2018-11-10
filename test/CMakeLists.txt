cmake_minimum_required(VERSION 3.12)
include(${PROJECT_SOURCE_DIR}/cmake/DownloadProject/DownloadProject.cmake)
download_project(PROJ googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        )

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

include(GoogleTest)

add_executable(TestCPU cpu_test.cpp ${PROJECT_SOURCE_DIR}/src/memory.cpp)
target_link_libraries(TestCPU cpu gtest_main)
gtest_add_tests(TARGET TestCPU)

add_executable(TestINES ines_test.cpp)
target_link_libraries(TestINES ines gtest_main)
gtest_add_tests(TARGET TestINES)

add_executable(
    TestConsole console_test.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu.cpp
    ${PROJECT_SOURCE_DIR}/src/ines.cpp
    ${PROJECT_SOURCE_DIR}/src/memory.cpp
)
target_link_libraries(TestConsole console gtest_main)
gtest_add_tests(TARGET TestConsole)

add_executable(TestPPU ppu_test.cpp)
target_link_libraries(TestPPU ppu gtest_main)
gtest_add_tests(TARGET TestPPU)
