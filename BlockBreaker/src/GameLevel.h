#pragma once
#include <vector>

#include "GLEW/glew.h"
#include "glm/gtc/matrix_transform.hpp"

#include "GameObjects/GameObject.h"
#include "SpriteRenderer/SpriteRenderer.h"
#include "ResourceManager/ResourceManager.h"

class GameLevel
{
public:
	GameLevel() {};

	void Load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(SpriteRenderer& renderer);
	bool isCompleted();

	std::vector <GameObject>& GetBricks() { return m_Bricks; }

private:
	std::vector <GameObject> m_Bricks;

	void init(std::vector<std::vector<unsigned int>>& tileData, unsigned int levelWidth, unsigned int levelHeight);
};

