project(vector-math LANGUAGES C CXX)
add_executable(vector-math vector-math.cpp ../src/movement.cpp)

project(movement-math LANGUAGES C CXX)
add_executable(movement-math movement-math.cpp ../src/movement.cpp)