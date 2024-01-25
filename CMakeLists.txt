cmake_minimum_required(VERSION 3.2)

project(Rhobot
        VERSION 0.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(JetsonGPIO)

set(motor_test motor_test)

add_executable(${motor_test} ${CMAKE_CURRENT_SOURCE_DIR}/tests/motor_test.cpp)

# sets it so that this target is built in the tests folder
set_target_properties( motor_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

target_link_libraries(${motor_test} JetsonGPIO::JetsonGPIO)