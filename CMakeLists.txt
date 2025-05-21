cmake_minimum_required(VERSION 3.12)
project(PingPong2D VERSION 0.1.0 LANGUAGES C CXX)

set(PROJECT_NAME PingPong2D)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/game/game.hpp 
    src/game/game.cpp 
    src/game/game_object.hpp 
    src/game/ball.hpp 
    src/game/player.hpp
)

add_subdirectory(avion ${CMAKE_CURRENT_BINARY_DIR}/build)
target_link_libraries(${PROJECT_NAME} PUBLIC AvionEngineCore)

