cmake_minimum_required(VERSION 3.2)

project(Rhobot
        VERSION 0.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# build the test executables
add_subdirectory(tests)

# build the demo executables
add_subdirectory(demos)