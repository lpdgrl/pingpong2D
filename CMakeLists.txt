cmake_minimum_required(VERSION 3.10.0)
project(PingPong2D VERSION 0.1.0 LANGUAGES C CXX)

add_executable(PingPong2D src/main.cpp src/game.hpp src/game.cpp src/render.hpp src/render.cpp src/shader.hpp src/shader.cpp src/glad.c)

# glfw
include_directories("/home/lpdgrl/Project/code/pingpong2D/src/includes/")

# glad 
include_directories("/home/lpdgrl/Project/code/pingpong2D/src/includes/glad/")
include_directories("/home/lpdgrl/Project/code/pingpong2D/src/includes/glad/khr")

# link libglfw3.a
target_link_libraries(PingPong2D "/home/lpdgrl/Project/code/pingpong2D/src/libs/libglfw3.a")