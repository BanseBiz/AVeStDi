cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
project(av-data-distributer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(GeographicLib REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories (${GeographicLib_INCLUDE_DIRS})

include(CTest)
add_subdirectory(tests)
enable_testing()

include(${CMAKE_CURRENT_SOURCE_DIR}/lib/CMakeLists.txt)

add_executable(av-data-distributer src/main.cpp src/vehicle.cpp src/movement.cpp)
target_link_libraries(av-data-distributer uWebSockets ${GeographicLib_LIBRARIES} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_test(
  NAME vector-math
  COMMAND vector-math
)

add_test(
  NAME movement-math
  COMMAND movement-math
)