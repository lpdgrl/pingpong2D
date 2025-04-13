cmake_minimum_required(VERSION 3.10.0)
project(PingPong2D VERSION 0.1.0 LANGUAGES C CXX)

add_executable(PingPong2D 
src/main.cpp 
src/game/game.hpp 
src/game/game.cpp 
src/engine/render.hpp 
src/engine/render.cpp 
src/engine/shader.hpp 
src/engine/shader.cpp 
src/game/game_object.hpp 
src/game/ball.hpp 
src/game/player.hpp
src/glad.c)

# glfw
include_directories("/home/lpdgrl/Project/code/pingpong2D/src/includes/")

# glad 
include_directories("/home/lpdgrl/Project/code/pingpong2D/src/includes/glad/")
include_directories("/home/lpdgrl/Project/code/pingpong2D/src/includes/glad/khr")

# link libglfw3.a
target_link_libraries(PingPong2D "/home/lpdgrl/Project/code/pingpong2D/src/libs/libglfw3.a")