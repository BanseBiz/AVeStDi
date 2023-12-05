cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
project(av-data-distributer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package (GeographicLib REQUIRED)
include_directories (${GeographicLib_INCLUDE_DIRS})

include(CTest)
enable_testing()

add_subdirectory(lib)
add_executable(av-data-distributer src/main.cpp src/vehicle.cpp)
target_link_libraries(av-data-distributer uWebSockets ${GeographicLib_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
