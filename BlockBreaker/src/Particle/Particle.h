#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "../Utility/Shader.h"
#include "../Utility/Texture.h"
#include "../GameObjects/GameObject.h"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator
{
public:
    ParticleGenerator(const Shader& shader, const Texture& texture, unsigned int amount);

    void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();

private:
    std::vector<Particle> m_Particles;
    unsigned int m_Amount;

    Shader m_Shader;
    Texture m_Texture;
    unsigned int m_VAO;
    unsigned int m_IB;
    unsigned int m_LastUsedParticle;

private:
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};