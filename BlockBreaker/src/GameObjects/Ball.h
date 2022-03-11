#pragma once
#include "GameObject.h"
#include "glm/gtc/matrix_transform.hpp"

class Ball : public GameObject
{
public:
	Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite);

	void Move(float dt, unsigned int window_width);
	void Reset(glm::vec2 pos, glm::vec2 vel);

	inline bool GetBallStuck() { return m_Stuck; }
	inline bool GetBallSticky() { return m_Sticky; }
	inline bool GetPassThrough() { return m_PassThrough; }
	inline float GetRadius() { return m_Radius; }

	void SetBallStuck(bool stuck) { m_Stuck = stuck; }
	void SetBallSticky(bool stick) { m_Sticky = stick; }
	void SetPassThrough(bool p) { m_PassThrough = p; }

private:
	float m_Radius;
	bool m_Stuck, m_Sticky, m_PassThrough;
};

