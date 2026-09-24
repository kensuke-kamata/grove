add_executable(hello hello.cpp)
add_test(NAME hello COMMAND hello)
set_tests_properties(hello PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello, grove!"
)

add_executable(file_modes file_modes.cpp)
add_test(NAME file_modes COMMAND file_modes)

add_executable(scope_resolution scope_resolution.cpp)
add_test(NAME scope_resolution COMMAND scope_resolution)

add_executable(object_lifetime object_lifetime.cpp)
add_test(NAME object_lifetime COMMAND object_lifetime)

add_executable(flagset flagset.cpp)
target_include_directories(flagset PRIVATE ${PROJECT_SOURCE_DIR}/include)
add_test(NAME flagset COMMAND flagset)
