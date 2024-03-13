cmake_minimum_required(VERSION 3.2)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(Rhobot
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(JetsonGPIO)
find_package(Threads REQUIRED)

### SET UP RHOBOT LIB ###

set(LIBSRC rhobot.cpp)
set(LIBINCLUDE rhobot.h)

add_library(rhobot
  STATIC
  ${LIBSRC}
  )

set_target_properties(rhobot PROPERTIES
  POSITION_INDEPENDENT_CODE TRUE
  PUBLIC_HEADER "${LIBINCLUDE}")

target_link_libraries(rhobot JetsonGPIO::JetsonGPIO)

install(
  TARGETS rhobot EXPORT rhobot-targets
  LIBRARY DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

### COMPILE EXECUTABLES ###

set(_executable_targets
    "motor_test"
    "differential_steering_demo"
    )

foreach (executables ${_executable_targets})
    add_executable(${executables} ${executables}.cpp)
    target_link_libraries(${executables} rhobot)

endforeach ()

add_custom_target(executables)
add_dependencies(executables ${_executable_targets})