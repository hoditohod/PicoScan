cmake_minimum_required(VERSION 3.20)

project(
    PicoScan
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


option(BUILD_TERMUX_GUI "Build test executable for Termux GUI" OFF)
option(BUILD_TESTS "Build unit test executable" ON)


add_library( render OBJECT
    render.cpp
    sbuffer.cpp
)

target_compile_options( render
    PUBLIC -Wall -fsanitize=address
)


# target_compile_options( render_test
#    PRIVATE -Wall
# )


if (BUILD_TERMUX_GUI)
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
endif (BUILD_TERMUX_GUI)


if (BUILD_TESTS)
    find_package(GTest REQUIRED)

    add_executable ( unit_test
        $<TARGET_OBJECTS:render>
        test_fix.cpp
        test_raster.cpp
    )

    target_compile_definitions( unit_test
        PRIVATE TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test_resources/")

    target_link_libraries( unit_test
        GTest::gtest
        GTest::gtest_main
    )

    target_link_options( unit_test
        PUBLIC -fsanitize=address
    )

    # target_compile_options( unit_test
    #    PRIVATE -Wall
    # )
endif (BUILD_TESTS)

