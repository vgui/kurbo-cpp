cmake_minimum_required(VERSION 3.16)
project(kurbo-cpp VERSION 0.11.2 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Find required packages
find_package(GTest REQUIRED)

# Create kurbo library
set(KURBO_SOURCES
    src/kurbo/point.cpp
    src/kurbo/vec2.cpp
    src/kurbo/size.cpp
    src/kurbo/common.cpp
    src/kurbo/rect.cpp
    src/kurbo/rounded_rect.cpp
    src/kurbo/rounded_rect_radii.cpp
    src/kurbo/line.cpp
    src/kurbo/circle.cpp
    src/kurbo/ellipse.cpp
    src/kurbo/triangle.cpp
    src/kurbo/affine.cpp
    src/kurbo/translate_scale.cpp
)

add_library(kurbo STATIC ${KURBO_SOURCES})
target_include_directories(kurbo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set library properties
set_target_properties(kurbo PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "include/kurbo/kurbo.hpp"
)

# Create test executable
enable_testing()
add_executable(kurbo_tests
    tests/test_point.cpp
    tests/test_vec2.cpp
    tests/test_size.cpp
    tests/test_common.cpp
    tests/test_affine.cpp
    tests/test_translate_scale.cpp
    tests/test_rect.cpp
    tests/test_rounded_rect.cpp
    tests/test_rounded_rect_radii.cpp
    tests/test_line.cpp
    tests/test_circle.cpp
    tests/test_ellipse.cpp
    tests/test_triangle.cpp
)

target_link_libraries(kurbo_tests
    kurbo
    GTest::gtest
    GTest::gtest_main
)

# Add tests
add_test(NAME PointTests COMMAND kurbo_tests --gtest_filter=PointTest*)
add_test(NAME Vec2Tests COMMAND kurbo_tests --gtest_filter=Vec2Test*)
add_test(NAME SizeTests COMMAND kurbo_tests --gtest_filter=SizeTest*)
add_test(NAME CommonTests COMMAND kurbo_tests --gtest_filter=CommonTest*)
add_test(NAME AffineTests COMMAND kurbo_tests --gtest_filter=AffineTest*)
add_test(NAME TranslateScaleTests COMMAND kurbo_tests --gtest_filter=TranslateScaleTest*)
add_test(NAME RectTests COMMAND kurbo_tests --gtest_filter=RectTest*)
add_test(NAME RoundedRectTests COMMAND kurbo_tests --gtest_filter=RoundedRectTest*)
add_test(NAME RoundedRectRadiiTests COMMAND kurbo_tests --gtest_filter=RoundedRectRadiiTest*)
add_test(NAME LineTests COMMAND kurbo_tests --gtest_filter=LineTest*)
add_test(NAME CircleTests COMMAND kurbo_tests --gtest_filter=CircleTest*)
add_test(NAME EllipseTests COMMAND kurbo_tests --gtest_filter=EllipseTest*)
add_test(NAME TriangleTests COMMAND kurbo_tests --gtest_filter=TriangleTest*)

# Create examples directory if it doesn't exist
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

# Example: Basic geometry operations
add_executable(example_basic examples/basic.cpp)
target_link_libraries(example_basic kurbo)

# Example: Point and vector operations
add_executable(example_point_vec examples/point_vec.cpp)
target_link_libraries(example_point_vec kurbo)

# Example: Size operations
add_executable(example_size examples/size.cpp)
target_link_libraries(example_size kurbo)

# Example: Common operations
add_executable(example_common examples/common.cpp)
target_link_libraries(example_common kurbo)

# Example: Line operations
add_executable(example_line examples/line_example.cpp)
target_link_libraries(example_line kurbo)

# Example: Circle operations
add_executable(example_circle examples/circle_example.cpp)
target_link_libraries(example_circle kurbo)

# Example: Triangle operations
add_executable(example_triangle examples/triangle_example.cpp)
target_link_libraries(example_triangle kurbo)

# Example: Ellipse operations
add_executable(example_ellipse examples/ellipse_example.cpp)
target_link_libraries(example_ellipse kurbo)

# Example: Ellipse debug
add_executable(example_ellipse_debug examples/ellipse_debug.cpp)
target_link_libraries(example_ellipse_debug kurbo)

# Install rules
install(TARGETS kurbo
    EXPORT kurboTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY include/kurbo
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# Export targets
install(EXPORT kurboTargets
    FILE kurboTargets.cmake
    NAMESPACE kurbo::
    DESTINATION lib/cmake/kurbo
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    kurboConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/kurboConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/kurboConfig.cmake"
    INSTALL_DESTINATION lib/cmake/kurbo
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kurboConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/kurboConfigVersion.cmake"
    DESTINATION lib/cmake/kurbo
)

# Print configuration summary
message(STATUS "kurbo-cpp configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}") 