#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath> 
#include "shader.hpp"

float proccessKeyOfMove(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.vs";
const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.fs";

float move = 0.0f;

int main () {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PingPong 2D", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    
    Shader shader(PATH_TO_FILE_VERTEX_SHADER, PATH_TO_FILE_FRAGMENT_SHADER);

    float verticesPlayer[] = {
        // positions         // colors
        0.1f,  0.2f, 0.0f,  1.0f, 1.0f, 1.0f,  // top right
        0.1f, -0.2f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom right
       -0.1f, -0.2f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom left
       -0.1f,  0.2f, 0.0f,   1.0f, 1.0f, 1.0f   // top left 
    };

    unsigned int indicesPlayer[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlayer), verticesPlayer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlayer), indicesPlayer, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // scale of model player
    shader.use();
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 1.0f, 0.0f));
    glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -3.0f));
    glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;

    
    shader.setMat4("model", model_matrix);
    // shader.setMat4("model", model);
    // shader.setMat4("view", view);


    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        move = proccessKeyOfMove(window);

        std::cout << "move: " << move << std::endl;
        glm::mat4 rotate_mode = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        
        model = model * rotate_mode;

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        shader.setMat4("view", view);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    return 0;
}

float proccessKeyOfMove(GLFWwindow* window) {
    float velocity = 10.0f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        move += 0.1f * velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        move -= 0.1 * velocity;
    }
    return move;
}


// callback function for resizing window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        std::cout << "Window been closed!" << std::endl;
    }
}