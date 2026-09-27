add_executable(raylib_window raylib_window/main.cpp)
target_link_libraries(raylib_window PRIVATE raylib)

add_executable(alpha_blending alpha_blending/main.cpp)
target_link_libraries(alpha_blending PRIVATE raylib)

file(GLOB SO_LONG_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/so_long/*.cpp"
)

add_executable(so_long application.cpp ${SO_LONG_SOURCES})
target_include_directories(so_long PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(so_long PRIVATE raylib)
