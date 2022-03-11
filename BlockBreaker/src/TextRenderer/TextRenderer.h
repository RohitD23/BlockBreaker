#pragma once
#include <unordered_map>

#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include "../Utility/Shader.h"
#include "../Utility/Texture.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // size of glyph
    glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
    unsigned int Advance;   // horizontal offset to advance to next glyph
};

// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
    TextRenderer(const glm::mat4& proj);
    // pre-compiles a list of characters from the given font
    void Load(std::string font, unsigned int fontSize);
    // renders a string of text using the precompiled list of characters
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
    // holds a list of pre-compiled Characters
    std::unordered_map<char, Character> m_Characters;
    // shader used for text rendering
    Shader m_TextShader;
    // render state
    unsigned int m_VAO, m_VBO, m_IB;
};

