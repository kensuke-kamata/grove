add_executable(hello hello.cpp)

add_test(NAME hello COMMAND hello)
set_tests_properties(hello PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello, grove!"
)
