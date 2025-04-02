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
float movePlayer = 0.1f;
float mvBallX = 0.01f;
float mvBallY = 0.01f;
float currentBallPosX = 0.0f;
float currentBallPosY = 0.0f;
float ballPosX = 0.1f;
float ballPosY = 0.1f;
float velocityBall = 0.012f;
float velocityPlayer = 0.25f;
bool isStartedGame = false;
bool firstMoveBall = true;

glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);



int main () {
    
    using namespace std::literals;
    
    Game game(name_window, SCR_WIDTH, SCR_HEIGHT);
    std::cout << "d" << std::endl;
    game.InitGame();
    
    GLFWwindow* window = game.GetPointerWindow();
    
    
    
    // Shader* shader = new Shader(PATH_TO_FILE_VERTEX_SHADER, PATH_TO_FILE_FRAGMENT_SHADER);

    Render* render = game.GetRenderPointer();

    Shader* shader = render->GetShaderPointer();

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    // Shader shader_ball(PATH_TO_FILE_VERTEX_SHADER_BALL, PATH_TO_FILE_FRAGMENT_SHADER_BALL);

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
   /* shader_ball.use();
    shader_ball.setInt("textureball", 0);
    
    float angleBall = randomFloat(0.0f, 360.0f);
    std::cout << "angleBall: " << angleBall << std::endl;

    glm::vec3 scaleBall = glm::vec3(1.0f, 0.2f, 0.0f);
    glm::vec3 positionBall = glm::vec3(0.1f, 0.1f, 0.0f);
    glm::vec3 rotateBall = glm::vec3(0.0f, 0.0f, 1.0f);

    // vec of player
    glm::vec3 scalePlayer = glm::vec3(0.2f, 0.5f, 1.0f);
    glm::vec3 positionPlayer = glm::vec3(-1.0f, detectPlayerColOfEndScreen(movePlayer), 0.0f);
    glm::vec3 rotatePlayer = glm::vec3(0.0f, 0.0f, 1.0f);
    
    glm::vec3 viewVec = glm::vec3(0.0f, 0.0f, -2.0f);
    float fov = 0.0f;

    int direction = 0;
    */

    glm::mat4 modelMatrix = glm::mat4(1.f);
    
    modelMatrix = glm::translate(modelMatrix, glm::vec3(400.f, 300.f, 0.f));
    glm::mat4 modelMatrix_ = glm::translate(glm::mat4(1.f), glm::vec3(100.f, 100.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(SCR_WIDTH), 0.f, static_cast<float>(SCR_HEIGHT), -100.f, 100.f);
    
    shader->use();
    shader->setMat4("projectionMat", projectionMatrix);
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);


        glm::vec3 v(100.0f, 100.0f, 0.0f);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //shader->use();
        // glBindVertexArray(render->GetVAO(0));
        

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            v.y += 10.f;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            v.y -= 10.f;
        }

        game.Update(v);

        //modelMatrix = glm::translate(modelMatrix, v);
        //shader->setMat4("modelMat", modelMatrix);
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //modelMatrix_ = glm::translate(modelMatrix_, v);
        //shader->setMat4("modelMat", modelMatrix_);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

        /*
        movePlayer = proccessKeyOfMove(window);

        // matrix of view and projection
        // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        moveCamera(window, viewVec);
        changeFOV(window, fov);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), viewVec);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        std::cout << "Camera xPos: " << viewVec.x << " yPos: " << viewVec.y << " zPos: " << viewVec.z << std::endl;
        std::cout << "fov: " << fov << std::endl;

        // model of player
        positionPlayer.y = detectPlayerColOfEndScreen(movePlayer);
        glm::mat4 scaleMatrixPlayer = glm::scale(glm::mat4(1.0f), scalePlayer);
        glm::mat4 rotateMatrixPlayer = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), rotatePlayer);
        glm::mat4 translateMatrixPlayer = glm::translate(glm::mat4(1.0f), positionPlayer);

        glm::mat4 modelMatrixPlayer = translateMatrixPlayer * rotateMatrixPlayer * scaleMatrixPlayer;
        glm::mat4 matrixProductPlayer = projection * view * modelMatrixPlayer;
        
        shader.setMat4("matrixProductPlayer", matrixProductPlayer);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureball);

        int xPlayer = (positionPlayer.x + 1) * positionPlayer.x * SCR_WIDTH;
        int yPlayer = (positionPlayer.y + 1) * positionPlayer.y * SCR_HEIGHT;
        std::cout << "xPos player: " << xPlayer << " yPos player: " << yPlayer << std::endl;

        int xBall = (positionBall.x + 1) * positionBall.x * SCR_WIDTH;
        int yBall = (positionBall.y + 1) * positionBall.y * SCR_HEIGHT;
        std::cout << "xPos ball: " << xBall << " yPos ball: " << xBall << std::endl;

        // moving ball
        detectCollisionPlayerAndBall(positionPlayer, positionBall, Velocity);
        detectCollisionEndScreenBall(positionBall, Velocity);
        moveBall(Velocity, positionBall, direction, angleBall);
        
        glm::mat4 scaleMatrixBall = glm::scale(glm::mat4(1.0f), scaleBall);
        glm::mat4 rotateMatrixBall = glm::rotate(glm::mat4(1.0f), glm::radians(angleBall), rotateBall);
        glm::mat4 translateMatrixBall = glm::translate(glm::mat4(1.0f), positionBall);

        glm::mat4 modelMatrixBall = translateMatrixBall * rotateMatrixBall * scaleMatrixBall;
        glm::mat4 matrixProductBall = projection * view * modelMatrixBall;

        shader_ball.use();
        shader_ball.setMat4("matrixProductBall", matrixProductBall);

        glBindVertexArray(VAO_ball);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        */
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    return 0;
}

void moveBall(glm::vec3 &Velocity, glm::vec3 &positionBall, int &direction, float &angleBall) {
    if (isStartedGame) {
        if (firstMoveBall) {
            Velocity = glm::vec3(0.012f, 0.01f, 0.0f);
            direction = randomNegativeInt(-1.0f, 1.0f);

            PrintVXYAD(Velocity.x, Velocity.y, angleBall, direction);

            positionBall.x = moveBallX(positionBall.x, Velocity.x, direction, angleBall);
            positionBall.y = moveBallY(positionBall.y, Velocity.y, direction, angleBall);
            firstMoveBall = false;
        }
        else {
                PrintVXYAD(Velocity.x, Velocity.y, angleBall, direction);

                positionBall.x = moveBallX(positionBall.x, Velocity.x, direction, angleBall);
                positionBall.y = moveBallY(positionBall.y, Velocity.y, direction, angleBall);
        }
    }
    else {
        if (!firstMoveBall) {
            angleBall = randomFloat(0.0f, 360.0f);
        }
        positionBall.x = 0.01f;
        positionBall.y = 0.01f;
        firstMoveBall = true;   
    }
}

void startGame() {
    isStartedGame = true;
}

void resetGame() {
    isStartedGame = false;
}

void detectCollisionPlayerAndBall(glm::vec3 &positionPlayer, glm::vec3 &positionBall, glm::vec3 &Velocity) {
    if (positionPlayer.y >= positionBall.y * 10.0f && positionPlayer.x * 10.0f >= positionBall.x * 10.0f) {
        Velocity.y *= -1;
        Velocity.x *= -1;
    }
}

void detectCollisionEndScreenBall(glm::vec3 &positionBall, glm::vec3 &Velocity) {
    if (positionBall.y >= 0.8f) {
        positionBall.y = 0.75f;
        Velocity.y *= -1;
    }
    else if (positionBall.y <= -0.8f) {
        positionBall.y = -0.75f;
        Velocity.y *= -1;
    }
}

float moveBallX(float posX, float velocity, int direction, float angle) {
    if (direction == 1) {
        posX += velocity;
    }
    else if (direction == -1) {
        posX -= velocity;
    }
    return posX;
}

float moveBallY(float posY, float velocity, int direction, float angle) {
    if (direction == 1) {
        posY += velocity;
    }
    else if (direction == -1) {
        posY -= velocity;
    }
    return posY;
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

void changeFOV(GLFWwindow* window, float &fov) {
    float velocity = 0.5f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        fov += velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        fov -= velocity;
    }
}

void moveCamera(GLFWwindow* window, glm::vec3 &posCamera) {
    float velocity = 0.01f;

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        posCamera.y += velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        posCamera.y -= velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        posCamera.x += velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        posCamera.x -= velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        posCamera.z += velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        posCamera.z -= velocity;
    }
}

float proccessKeyOfMove(GLFWwindow* window) {
    float velocity = 0.25f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movePlayer += 0.1f * velocity;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movePlayer -= 0.1 * velocity;
    }
    return movePlayer;
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

void PrintVXYAD(float x, float y, float angle, int direction) {
    std::cout << "Velocity.x: " << x << std::endl;
    std::cout << "Velocity.y: " << y << std::endl;

    std::cout << "AngleBall: " << glm::radians(angle) << std::endl;
    std::cout << "Direction: " << direction << std::endl;
}
