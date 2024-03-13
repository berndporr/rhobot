find_package(JetsonGPIO)

# set the targets which shouold be compiled
set(_test_targets
    "motor_test"
    "pin_out_test"
    )

foreach (test ${_test_targets})
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} JetsonGPIO::JetsonGPIO)

endforeach ()

add_custom_target(tests)
add_dependencies(tests ${_test_targets})