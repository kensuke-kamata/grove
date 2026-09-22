add_executable(hello hello.cpp)
add_test(NAME hello COMMAND hello)
set_tests_properties(hello PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello, grove!"
)

add_executable(file_modes file_modes.cpp)
add_test(NAME file_modes COMMAND file_modes)

add_executable(scope_resolution scope_resolution.cpp)
add_test(NAME scope_resolution COMMAND scope_resolution)
