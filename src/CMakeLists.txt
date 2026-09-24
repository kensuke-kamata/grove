add_executable(raylib_window raylib_window/main.cpp)
target_link_libraries(raylib_window PRIVATE raylib)

add_executable(alpha_blending alpha_blending/main.cpp)
target_link_libraries(alpha_blending PRIVATE raylib)
