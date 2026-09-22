add_executable(hello hello.cpp)
add_executable(file_modes file_modes.cpp)

add_test(NAME hello COMMAND hello)
set_tests_properties(hello PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello, grove!"
)
