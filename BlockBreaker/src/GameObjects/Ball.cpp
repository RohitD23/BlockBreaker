#include "Ball.h"

Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite)
	:GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity),
	 m_Radius(radius), m_Stuck(true), m_Sticky(false), m_PassThrough(false){}

void Ball::Move(float dt, unsigned int window_width)
{
	if (!m_Stuck) 
	{
		m_Position += m_Velocity * dt;

		if (m_Position.x <= 0.0f) 
		{
			m_Velocity.x = -m_Velocity.x;
			m_Position.x = 0.0f;
		}
		else if (m_Position.x + m_Size.x >= window_width)
		{
			m_Velocity.x = -m_Velocity.x;
			m_Position.x = window_width - m_Size.x;
		}

		if (m_Position.y <= 0.0f)
		{
			m_Velocity.y = -m_Velocity.y;
			m_Position.y = 0.0f;
		}
	}
}

void Ball::Reset(glm::vec2 pos, glm::vec2 vel)
{
	m_Position = pos;
	m_Velocity = vel;
	m_Stuck = true;
	m_Sticky = false;
	m_PassThrough = false;
}
