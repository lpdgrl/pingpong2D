 // Shader shader_ball(PATH_TO_FILE_VERTEX_SHADER_BALL, PATH_TO_FILE_FRAGMENT_SHADER_BALL);

    /*
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
   //shader_ball.use();
    //shader_ball.setInt("textureball", 0);
    */