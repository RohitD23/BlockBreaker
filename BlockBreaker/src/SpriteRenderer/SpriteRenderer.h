#pragma once
#include "GLEW/glew.h"
#include "glm/gtc/matrix_transform.hpp"

#include "../Utility/Shader.h"
#include "../Utility/Texture.h"

class SpriteRenderer
{
public:
    SpriteRenderer(const Shader& shader);
    ~SpriteRenderer() {};

    void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:

    Shader m_Shader;
    unsigned int m_VAO;
    unsigned int m_IB;

private:
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

