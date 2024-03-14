cmake_minimum_required(VERSION 3.14.0 FATAL_ERROR)
project(avestdi VERSION 0.1.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(AVELIB_DIR lib/AVeLib)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(GeographicLib REQUIRED)
include_directories (${GeographicLib_INCLUDE_DIRS})

include(${CMAKE_CURRENT_SOURCE_DIR}/lib/CMakeLists.txt)

include_directories( ${AVELIB_DIR}/include )

add_library( avelib ${AVELIB_DIR}/src/movement.cpp ${AVELIB_DIR}/src/vehicle.cpp )

add_executable(avestdi src/main.cpp src/tcpSocket.cpp src/storage.cpp src/cmdHandler.cpp)
target_link_libraries(avestdi GeographicLib avelib uWebSockets ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
