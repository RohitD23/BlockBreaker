#include "Collison.h"
#include "glm/gtc/matrix_transform.hpp"

bool CheckCollision(GameObject& one, GameObject& two)
{

	// collision x-axis
	bool collisionX = (one.GetPosition().x + one.GetSize().x >= two.GetPosition().x) &&
		(two.GetPosition().x + two.GetSize().x >= one.GetPosition().x);
	// collision y-axis
	bool collisionY = (one.GetPosition().y + one.GetSize().y >= two.GetPosition().y) &&
		(two.GetPosition().y + two.GetSize().y >= one.GetPosition().y);

	// collision only if on both axes
	return collisionX && collisionY;
}

Collision CheckCollision(Ball& one, GameObject& two)
{
	// get center point circle first
	glm::vec2 center(one.GetPosition() + one.GetRadius());

	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.GetSize().x / 2.0f, two.GetSize().y / 2.0f);
	glm::vec2 aabb_center(two.GetPosition().x + aabb_half_extents.x,
		two.GetPosition().y + aabb_half_extents.y);

	// get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents,
		aabb_half_extents);

	// add clamped value to AABB_center and get the value closest to circle
	glm::vec2 closest = aabb_center + clamped;

	// vector between center circle and closest point AABB
	difference = closest - center;
	if (glm::length(difference) <= one.GetRadius())
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));

}

Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = 
	{
		glm::vec2(0.0f, 1.0f),  //UP
		glm::vec2(0.0f, -1.0f), //DOWN
		glm::vec2(1.0f, 0.0f),  //RIGHT
		glm::vec2(-1.0f, 0.0f)  //LEFT
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;
	for (int i = 0; i < 4; i++) 
	{
		float dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max) 
		{
			max = dotProduct;
			bestMatch = i;
		}
	}
	return Direction(bestMatch);
}
