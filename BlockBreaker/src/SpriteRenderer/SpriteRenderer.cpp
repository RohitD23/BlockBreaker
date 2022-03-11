#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const Shader& shader)
    :m_Shader(shader)
{
    initRenderData();
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    m_Shader.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    m_Shader.SetUniformMat4f("u_model", model);

    // render textured quad
    m_Shader.SetUniform3f("u_spriteColor", color);
    texture.Bind();

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0
    };

    glGenVertexArrays(1, &m_VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

