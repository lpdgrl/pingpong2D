#include "image_loader.hpp"
#include "game/game.hpp"

void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* name_window = "Ping Pong 2D";
const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.vs";
const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.fs";
const char* textureEARTHBALL = "/home/lpdgrl/Project/code/pingpong2D/data/textures/fireball.png";

int main () {
    
    using namespace std::literals;
    
    Game game(name_window, SCR_WIDTH, SCR_HEIGHT);

    game.InitGame();
    GLFWwindow* window = game.GetPointerWindow();
    
    game.StartGame();

    GLfloat delta_time = 0.f;
    GLfloat last_frame = 0.f;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::cout << "Delta time: " << delta_time << std::endl;
        game.Update(delta_time);

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


