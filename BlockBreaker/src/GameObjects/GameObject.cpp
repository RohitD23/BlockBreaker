#include "GameObject.h"

GameObject::GameObject()
    : m_Position(0.0f, 0.0f), m_Size(1.0f, 1.0f), m_Velocity(0.0f),
    m_Color(1.0f), m_Rotation(0.0f), m_Sprite(), m_IsSolid(false), m_Destroyed(false) { };

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color, glm::vec2 velocity)
    : m_Position(pos), m_Size(size), m_Velocity(velocity), m_Color(color), 
    m_Rotation(0.0f), m_Sprite(sprite), m_IsSolid(false), m_Destroyed(false) { }

void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(m_Sprite, m_Position, m_Size, m_Rotation, m_Color);
}
