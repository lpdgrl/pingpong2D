#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath> 
#include "shader.hpp"
#include "image_loader.hpp"


float proccessKeyOfMove(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
float detectPlayerColOfEndScreen(float pos);
float moveBallX(float posX, float curMoveBall);
float moveBallY(float posY, float curMoveBall);
float randomFloat(float min, float max);
int randomNegativeInt(int min, int max);
void startGame();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.vs";
const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.fs";
const char* textureEARTHBALL = "/home/lpdgrl/Project/code/pingpong2D/data/textures/fireball.png";
const char* PATH_TO_FILE_VERTEX_SHADER_BALL = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader_ball.vs";
const char* PATH_TO_FILE_FRAGMENT_SHADER_BALL = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader_ball.fs";

// set settings of game
float movePlayer = 0.1f;
float mvBallX = 0.01f;
float mvBallY = 0.01f;
float currentBallPosX = 0.0f;
float currentBallPosY = 0.0f;
float ballPosX = 0.0f;
float ballPosY = 0.0f;
float velocityBall = 0.055f;
float velocityPlayer = 0.25f;
bool isStartedGame = false;
bool firstMoveBall = true;

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
       -0.1f,  0.2f, 0.0f,  1.0f, 1.0f, 1.0f   // top left 
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlayer), verticesPlayer, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlayer), indicesPlayer, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    Shader shader_ball(PATH_TO_FILE_VERTEX_SHADER_BALL, PATH_TO_FILE_FRAGMENT_SHADER_BALL);

    float verticesBall[] = {
        // positions
        0.1f,  0.2f, 0.0f, 1.0f, 1.0f,  // top right
        0.1f, -0.2f, 0.0f, 1.0f, 0.0f, // bottom right
       -0.1f, -0.2f, 0.0f, 0.0f, 0.0f, // bottom left
       -0.1f,  0.2f, 0.0f,  0.0f, 1.0f // top left 
    };

    unsigned int indicesBall[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO_ball, VAO_ball, EBO_ball;
    glGenVertexArrays(1, &VAO_ball);
    glGenBuffers(1, &VBO_ball);
    glGenBuffers(1, &EBO_ball);

    glBindVertexArray(VAO_ball);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_ball);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBall), verticesBall, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ball);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBall), indicesBall, GL_DYNAMIC_DRAW);

    // position of ball attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture of ball attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture 
    unsigned int textureball;
    glGenTextures(1, &textureball);
    glBindTexture(GL_TEXTURE_2D, textureball);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load file texture 
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureEARTHBALL, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    shader_ball.use();
    shader_ball.setInt("textureball", 0);
    
    float angleBall = randomFloat(5.0f, 360.0f);
    std::cout << "angleBall: " << angleBall << std::endl;
    // render loop
    while (!glfwWindowShouldClose(window)) {

        processInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        movePlayer = proccessKeyOfMove(window);

        // matrix of view and projection
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // model of player
        glm::vec3 scalePlayer = glm::vec3(0.2f, 0.8f, 1.0f);
        glm::vec3 positionPlayer = glm::vec3(-1.0f, detectPlayerColOfEndScreen(movePlayer), 0.0f);
        glm::vec3 rotatePlayer = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::mat4 scaleMatrixPlayer = glm::scale(glm::mat4(1.0f), scalePlayer);
        glm::mat4 rotateMatrixPlayer = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), rotatePlayer);
        glm::mat4 translateMatrixPlayer = glm::translate(glm::mat4(1.0f), positionPlayer);

        // std::cout << "pos.y = " << position.y << std::endl;

        glm::mat4 modelMatrixPlayer = translateMatrixPlayer * rotateMatrixPlayer * scaleMatrixPlayer;
        
        glm::mat4 matrixProductPlayer = projection * view * modelMatrixPlayer;
        shader.setMat4("matrixProductPlayer", matrixProductPlayer);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureball);
        
        shader_ball.use();
        if (isStartedGame) {
            if (firstMoveBall) {
                int random = randomNegativeInt(-1.0f, 1.0f);
                currentBallPosX = moveBallX(ballPosX, (randomFloat(1.0f, -1.0f) * random) / angleBall);
                currentBallPosY = moveBallY(ballPosY, (randomFloat(-0.5f, 0.5f) * random) / angleBall);
                ballPosX = currentBallPosX;
                ballPosY = currentBallPosY;
                firstMoveBall = false;
            }
            else {
                currentBallPosX = moveBallX(ballPosX, currentBallPosX);
                currentBallPosY = moveBallY(ballPosY, currentBallPosY);
                ballPosX = currentBallPosX;
                ballPosY = currentBallPosY;
            }
        }
        else {
            if (!firstMoveBall) {
                angleBall = randomFloat(50.0f, 360.0f);
            }
            currentBallPosX = ballPosX = 0.0f;
            currentBallPosY = ballPosY = 0.0f;
            mvBallX = 0.0f;
            mvBallY = 0.0f;
            firstMoveBall = true;
            
        }

        glm::vec3 scaleBall = glm::vec3(1.0f, 0.2f, 0.0f);
        glm::vec3 positionBall = glm::vec3(ballPosX, ballPosY, 0.0f);
        glm::vec3 rotateBall = glm::vec3(0.0f, 0.0f, 1.0f);
    
        glm::mat4 scaleMatrixBall = glm::scale(glm::mat4(1.0f), scaleBall);
        glm::mat4 rotateMatrixBall = glm::rotate(glm::mat4(1.0f), glm::radians(angleBall), rotateBall);
        glm::mat4 translateMatrixBall = glm::translate(glm::mat4(1.0f), positionBall);

        glm::mat4 modelMatrixBall = translateMatrixBall * rotateMatrixBall * scaleMatrixBall;
        glm::mat4 matrixProductBall = projection * view * modelMatrixBall;
        shader_ball.setMat4("matrixProductBall", matrixProductBall);

        glBindVertexArray(VAO_ball);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    return 0;
}

void startGame() {
    isStartedGame = true;
}

void resetGame() {
    isStartedGame = false;
}

float moveBallX(float posX, float curMoveBall) {
    curMoveBall += posX * velocityBall;
    return curMoveBall;
}

float moveBallY(float posY, float curMoveBall) {
    curMoveBall += posY * velocityBall;
    return curMoveBall;
}

float detectCollisionBallY(float posY) {
    float currPos = posY;
    if (posY >= 0.675f) {
        currPos = 0.675f;
    }
    else if (posY <= -0.65f) {
        currPos = -0.65f;
    }
    return currPos;
}

float detectPlayerColOfEndScreen(float pos) {
    float currPos = pos;
    if (pos >= 0.675f) {
        currPos = 0.675f;
    }
    else if (pos <= -0.65f) {
        currPos = -0.65f;
    }
    return currPos;
}

float proccessKeyOfMove(GLFWwindow* window) {
    float velocity = 0.25f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        movePlayer += 0.1f * velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        movePlayer -= 0.1 * velocity;
    }
    return movePlayer;
}

// callback function for resizing window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        std::cout << "Window been closed!" << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        startGame();
        return;
    }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        resetGame();
        return;
    }
}

float randomFloat(float min, float max) {
    srand(time(NULL));
    return min + (max - min) * (float)rand() / (float)RAND_MAX;
}

int randomNegativeInt(int min, int max) {
    srand(time(NULL));
    int random = 0;
    while (random == 0) {
        random = min + (rand() % (max - min + 1));
    }
    return random;
}
