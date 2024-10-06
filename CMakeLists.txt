cmake_minimum_required(VERSION 3.20)

project(
    render_test
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_library( render OBJECT
    render.cpp
)

target_compile_options( render
    PUBLIC -Wall -fsanitize=address
)


add_executable( render_termux
    $<TARGET_OBJECTS:render>
    main_termux.cpp
)

target_link_libraries( render_termux
    termuxgui
)


target_link_options( render_termux
    PUBLIC -fsanitize=address
)


# target_compile_options( render_test
#    PRIVATE -Wall
# )



add_executable ( unit_test
    $<TARGET_OBJECTS:render>
    test_fix.cpp
    test_raster.cpp
)

target_link_libraries( unit_test
    gtest
    gtest_main
)

target_link_options( unit_test
    PUBLIC -fsanitize=address
)

# target_compile_options( unit_test
#    PRIVATE -Wall
# )

