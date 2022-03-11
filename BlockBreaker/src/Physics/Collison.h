#pragma once
#include "../GameObjects/GameObject.h"
#include "../GameObjects/Ball.h"

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

bool CheckCollision(GameObject& one, GameObject& two); // AABB - AABB
Collision CheckCollision(Ball& one, GameObject& two); // AABB - Circle
Direction VectorDirection(glm::vec2 target);