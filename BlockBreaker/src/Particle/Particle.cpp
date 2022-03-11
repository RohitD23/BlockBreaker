#include "Particle.h"


ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture& texture, unsigned int amount)
    :m_Shader(shader), m_Texture(texture), m_Amount(amount), m_VAO(0), m_IB(0), m_LastUsedParticle(0)
{
    init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset)
{
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = firstUnusedParticle();
        respawnParticle(m_Particles[unusedParticle], object, offset);
    }
    // update all particles
    for (unsigned int i = 0; i < m_Amount; ++i)
    {
        Particle& p = m_Particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_Shader.Bind();
    for (const Particle& particle : m_Particles)
    {
        if (particle.Life > 0.0f)
        {
            m_Shader.SetUniform2f("u_offset", particle.Position);
            m_Shader.SetUniform4f("u_color", particle.Color);
            m_Texture.Bind();
            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glGenBuffers(1, &m_IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // set mesh attributes
    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create amount default particle instances
    for (unsigned int i = 0; i < m_Amount; ++i)
        m_Particles.push_back(Particle());
}

unsigned int ParticleGenerator::firstUnusedParticle()
{
    
    for (unsigned int i = m_LastUsedParticle; i < m_Amount; i++) {
        if (m_Particles[i].Life <= 0.0f)
        {
            m_LastUsedParticle = i;
            return i;
        }
    }

    // otherwise, do a linear search
    for (unsigned int i = 0; i < m_LastUsedParticle; i++) 
    {
        if (m_Particles[i].Life <= 0.0f)
        {
            m_LastUsedParticle = i;
            return i;
        }
    }

    m_LastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.GetPosition() + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.GetVelocity() * 0.1f;
}
