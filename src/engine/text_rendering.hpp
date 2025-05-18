#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad.h>

#include <map>
#include <iostream>

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

// Инвариант ?
class TextRender {
public:
    TextRender(const char* font_name);
    void Initialaztion();
    const Character& GetCharacter(GLchar c) const { return characters_.at(c); }
    

private:
    void LoadASCIISet();

private:
    const char* font_name_;
    std::map<GLchar, Character> characters_;
    FT_Library ft_;
    FT_Face face_;
};