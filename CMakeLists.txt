cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
project(av-data-distributer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()

add_subdirectory(lib)
add_executable(av-data-distributer src/main.cpp)
target_link_libraries(av-data-distributer uWebSockets)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
