find_package(JetsonGPIO)

# set the targets which shouold be compiled
set(_demo_targets
    "differential_steering_demo"
    )

foreach (demo ${_demo_targets})
    add_executable(${demo} ${demo}.cpp)
    target_link_libraries(${demo} JetsonGPIO::JetsonGPIO)

endforeach ()

add_custom_target(demos)
add_dependencies(demos ${_demo_targets})