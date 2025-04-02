#include <glad.h>
#include <glfw3.h>
#include <cmath> 
#include "image_loader.hpp"
#include "game.hpp"

float proccessKeyOfMove(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float detectPlayerColOfEndScreen(float pos);
float moveBallX(float posX, float velocity, int direction, float angle);
float moveBallY(float posY, float velocity, int direction, float angle);
float randomFloat(float min, float max);
int randomNegativeInt(int min, int max);
void startGame();
void resetGame();
void moveBall(glm::vec3 &Velocity, glm::vec3 &positionBall, int &direction, float &angleBall);
void detectCollisionEndScreenBall(glm::vec3 &positionBall, glm::vec3 &Velocity);
void detectCollisionPlayerAndBall(glm::vec3 &positionPlayer, glm::vec3 &positionBall, glm::vec3 &Velocity);
void moveCamera(GLFWwindow* window, glm::vec3 &posCamera);
void changeFOV(GLFWwindow* window, float &fov);
void PrintVXYAD(float x, float y, float angle, int direction);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* name_window = "Ping Pong 2D";
const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.vs";
const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.fs";
const char* textureEARTHBALL = "/home/lpdgrl/Project/code/pingpong2D/data/textures/fireball.png";


// set settings of game





int main () {
    
    using namespace std::literals;
    
    Game game(name_window, SCR_WIDTH, SCR_HEIGHT);
    std::cout << "d" << std::endl;
    game.InitGame();
    
    GLFWwindow* window = game.GetPointerWindow();
   
    game.StartGame();

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);


        glm::vec3 v(0.0f, 0.0f, 0.0f);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            v.y += 10.f;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            v.y -= 10.f;
        }

        game.Update({v.x, v.y});

       
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        std::cout << "Window been closed!" << std::endl;
    }
}


