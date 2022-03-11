#pragma once
#include "GLEW/glew.h"
#include "glm/gtc/matrix_transform.hpp"

#include "../Utility/Shader.h"
#include "../Utility/Texture.h"
#include "../SpriteRenderer/SpriteRenderer.h"

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, 
	glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(SpriteRenderer& renderer);

	inline bool GetIsSolid() const { return m_IsSolid; }
	inline bool GetDestroyed() const { return m_Destroyed; }

	inline glm::vec2 GetPosition() const { return m_Position; }
	inline glm::vec2 GetSize() const { return m_Size; }
	inline glm::vec2 GetVelocity() const { return m_Velocity; }

	void SetIsSolid(bool s) { m_IsSolid = s; }
	void SetDestroyed(bool d) { m_Destroyed = d; }

	void SetColor(glm::vec3 color) { m_Color = color; }

	void SetSize(glm::vec2 s) { m_Size = s; }
	void SetSizeX(float s) { m_Size.x = s; }

	void SetVelocity(glm::vec2 v) { m_Velocity = v; }
	void SetVelocityX(float x) { m_Velocity.x = x; }
	void SetVelocityY(float y) { m_Velocity.y = y; }
	void SetPosition(glm::vec2 p) { m_Position = p; }

	void ChangeSizeX(float s) { m_Size.x += s; }

	void ChangePositionX(float x) { m_Position.x += x; }
	void ChangePositionY(float y) { m_Position.y += y; }

	void ChangeVelocityDirectionX() { m_Velocity.x = -m_Velocity.x; }
	void ChangeVelocityDirectionY() { m_Velocity.y = -m_Velocity.y; }

protected:
	glm::vec2 m_Position, m_Size, m_Velocity;
	glm::vec3 m_Color;
	float m_Rotation;
	bool m_IsSolid;
	bool m_Destroyed;

	Texture m_Sprite;
};

