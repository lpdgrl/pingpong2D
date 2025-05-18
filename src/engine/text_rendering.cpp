#include "text_rendering.hpp"

TextRender::TextRender(const char* font_name) : font_name_{font_name} {}

void TextRender::Initialaztion() {
    if (FT_Init_FreeType(&ft_)) {
        std::cerr << "ERROR::FREETYPE Could not init FreeType Library" << std::endl;
    }

    if (FT_New_Face(ft_, font_name_, 0, &face_)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        FT_Set_Pixel_Sizes(face_, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        LoadASCIISet();
    }
}

void TextRender::LoadASCIISet() {
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face_, c, FT_LOAD_RENDER)){
            std::cerr << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // generate texture - переписать в отдельный класс Texture (чтобы возвращал уже созданный объект текстуры)
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face_->glyph->bitmap.width,
            face_->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face_->glyph->bitmap.buffer
        );
        
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows),
            glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
            static_cast<unsigned int>(face_->glyph->advance.x)
        };
        characters_.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face_);
    FT_Done_FreeType(ft_);
}