cmake_minimum_required(VERSION 3.2)

project(Rhobot
        VERSION 0.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(JetsonGPIO)

set(rotary_encoder_test rotary_encoder_test)

add_executable(${rotary_encoder_test} ${CMAKE_CURRENT_SOURCE_DIR}/tests/rotary_encoder_test.cpp)

# sets it so that this target is built in the tests folder
set_target_properties( rotary_encoder_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

target_link_libraries(${rotary_encoder_test} JetsonGPIO::JetsonGPIO)