# CMakeList.txt : CMake project for COP-4520-Assignments-Converted, include source and define
# project specific logic here.
#

SET(CMAKE_CXX_STANDARD 20)

cmake_minimum_required (VERSION 3.8)

project ("COP-4520-Assignments")

# Add source to this project's executable.
add_executable (COP-4520-Assignments "src/COP-4520-Assignments.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET COP-4520-Assignments PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(src)

target_include_directories(COP-4520-Assignments PUBLIC src)

target_link_directories(COP-4520-Assignments PRIVATE src)

target_link_libraries(COP-4520-Assignments src Common Assignments)

# TODO: Add tests and install targets if needed.
