#include "render.hpp"
#include <iostream>

void Render::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = CreateWindow(name_window_, scr_width_, scr_height_);

    glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_ = new Shader(PATH_TO_FILE_VERTEX_SHADER, PATH_TO_FILE_FRAGMENT_SHADER);
    shader_text_ = new Shader(PATH_TO_VERTEX_SHADER_TEXT, PATH_TO_FRAGMENT_SHADER_TEXT);
    text_ = new TextRender(PATH_TO_FONT);
    text_->Initialaztion();
}

void Render::InitRender() {
    float vertices_objects[] = {
        // positions         // colors
        1.f,  1.f, 0.0f,  1.0f, 1.0f, 1.0f,  // top right
        1.f,  -1.f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom right
        -1.f,  1.f, 0.0f,  .0f, 1.0f, .0f,  // bottom left
        -1.f, -1.f, 0.0f,  .0f, 1.0f, .0f   // top left 
    };

    unsigned int indices_objects[] = {
        0, 1, 3,
        0, 3, 2
    };
    
    //Buffers for objects
    GenerateBuffer(1, TypeBuffers::VAO, MapKey::OBJECTS);
    GenerateBuffer(1, TypeBuffers::VBO, MapKey::OBJECTS);
    GenerateBuffer(1, TypeBuffers::EBO, MapKey::OBJECTS);

    BindVertexArray(GetVAO(MapKey::OBJECTS));
    BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::OBJECTS));
    BufferData(GL_ARRAY_BUFFER, sizeof(vertices_objects), vertices_objects, GL_DYNAMIC_DRAW);

    BindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetEBO(MapKey::OBJECTS));
    BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_objects), indices_objects, GL_DYNAMIC_DRAW);

    // Attribute of position player
    EnableVertexAttribArray(0);
    SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Attribute of color player
    EnableVertexAttribArray(1);
    SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    BindBuffer(GL_ARRAY_BUFFER, 0);
    BindVertexArray(0);
}

void Render::InitRenderText() {
    GenerateBuffer(1, TypeBuffers::VAO, MapKey::TEXT);
    GenerateBuffer(1, TypeBuffers::VBO, MapKey::TEXT);

    BindVertexArray(GetVAO(MapKey::TEXT));
    BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::TEXT));
    
    BufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    EnableVertexAttribArray(0);
    SetVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    BindBuffer(GL_ARRAY_BUFFER, 0);
    BindVertexArray(0); 
}

GLFWwindow* Render::CreateWindow(const char* nw, unsigned int scr_w, unsigned int scr_h) {
        GLFWwindow* window = glfwCreateWindow(scr_w, scr_h, nw, NULL, NULL);
        if (window == NULL) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            // return -1;
        }

        glfwMakeContextCurrent(window);
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
        }  

        return window;
    }

// callback function for resizing window size
void Render::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Render::GenerateBuffer(const GLsizei n, TypeBuffers type, MapKey key) {
    GLuint b;
    
    switch(type)
    {
        case TypeBuffers::VAO: 
            glGenVertexArrays(n, &b);
            vao_.insert(std::make_pair(key, b));
        break;

        case TypeBuffers::VBO:
            glGenBuffers(n, &b);
            vbo_.insert(std::make_pair(key, b));
        break;

        case TypeBuffers::EBO:
            glGenBuffers(n, &b);
            ebo_.insert(std::make_pair(key, b));
        break;
    }
}

void Render::BindVertexArray(GLuint array) {
    glBindVertexArray(array);
}

void Render::BindBuffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void Render::BufferData(GLenum target, GLsizeiptr sizeptr, const void* data, GLenum usage) {
    glBufferData(target, sizeptr, data, usage);
}

void Render::SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void* offset) {
    glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
}

void Render::EnableVertexAttribArray(GLuint index) {
    glEnableVertexAttribArray(index);
}

void Render::SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
    glm::mat4 projectionMatrix = glm::ortho(left, static_cast<float>(scr_width_), bottom, static_cast<float>(scr_height_), zNear, zFar);

    shader_->use();
    shader_->setMat4("projectionMat", projectionMatrix);
    BindVertexArray(GetVAO(MapKey::OBJECTS));

    shader_text_->use();
    glUniformMatrix4fv(glGetUniformLocation(shader_text_->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    BindVertexArray(GetVAO(MapKey::TEXT));
}

void Render::Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate) {
    
    shader_->use();
    BindVertexArray(GetVAO(MapKey::OBJECTS));
    // Переписать в отдельные методы - тогда могу скалировать, вращать, перемещать независимо любой объект
    glm::mat4 model_matrix = glm::mat4(1.f);

    // Выполняем трансформации матрицы
    model_matrix = TranslateMatrix(model_matrix, position);
    model_matrix = RotateMatrix(model_matrix, axis, rotate);
    model_matrix = ScaleMatrix(model_matrix, size);

    shader_->setMat4("modelMat", model_matrix);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    BindVertexArray(GetVAO(MapKey::OBJECTS));
    
}

void Render::DrawText(std::string text, float x, float y, float scale, glm::vec3 color) {
    shader_text_->use();

    glUniform3f(glGetUniformLocation(shader_text_->ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    BindVertexArray(GetVAO(MapKey::TEXT));

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = text_->GetCharacter(*c);

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        BindBuffer(GL_ARRAY_BUFFER, GetVBO(MapKey::TEXT));
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        BindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    BindVertexArray(GetVAO(MapKey::TEXT));
}

glm::mat4 Render::RotateMatrix(glm::mat4& model, AxisRotate axis, GLfloat rotate) {
    glm::vec3 r_vec;

    switch(axis) {
        case AxisRotate::AXIS_X: {
            r_vec = {1.f, 0.f, 0.f};
            break;
        }
        case AxisRotate::AXIS_Y: {
            r_vec = {0.f, 1.f, 0.f};
            break;
        }
        case AxisRotate::AXIS_Z: {
            r_vec = {0.f, 0.f, 1.f};
            break;
        }
        case AxisRotate::NONE: {
            r_vec = {0.f, 0.f, 0.5f};
            break;
        }
    }
    return glm::rotate(model, glm::radians(rotate), r_vec);
}

glm::mat4 Render::TranslateMatrix(glm::mat4& model, const glm::vec2& position) {
    return glm::translate(model, glm::vec3(position, 0.f));
}

glm::mat4 Render::ScaleMatrix(glm::mat4& model, const glm::vec2& size) {
    return glm::scale(model, glm::vec3(size.x, size.y, 0.f));
}