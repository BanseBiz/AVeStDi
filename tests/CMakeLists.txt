project(vector-math LANGUAGES C CXX)
add_executable(vector-math vector-math.cpp ../src/movement.cpp)
target_link_libraries(vector-math ${GeographicLib_LIBRARIES})

project(movement-math LANGUAGES C CXX)
add_executable(movement-math movement-math.cpp ../src/movement.cpp)
target_link_libraries(movement-math ${GeographicLib_LIBRARIES})