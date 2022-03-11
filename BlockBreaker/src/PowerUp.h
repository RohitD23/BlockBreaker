#pragma once
#include "../GameObjects/GameObject.h"
#include "../Utility/Texture.h"

const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture texture)
        : GameObject(position, SIZE, texture, color, VELOCITY), m_Type(type), m_Duration(duration), m_Activated()
    { }

    inline const std::string GetType() const { return m_Type; }
    inline const bool GetActivated() const { return m_Activated; }
    inline const float GetDuration() const { return m_Duration; }

    void SetActivated(bool a) { m_Activated = a; }
    void Move(float dt) { m_Position += m_Velocity * dt; }
    void ChangeDuration(float dt) { m_Duration -= dt; }

private:
    std::string m_Type;
    float       m_Duration;
    bool        m_Activated;
};

