#include "GameLevel.h"

#include <fstream>
#include <sstream>

void GameLevel::Load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight)
{
	m_Bricks.clear();

	unsigned int tileCode;
	std::string line;
	std::ifstream stream(filepath);
	std::vector <std::vector<unsigned int>> tileData;

	if (stream) {
		while (std::getline(stream, line)) {
			std::stringstream ss(line);
			std::vector<unsigned int> row;
			while (ss >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : m_Bricks)
		if (!tile.GetDestroyed())
			tile.Draw(renderer);
}

bool GameLevel::isCompleted()
{
	for (GameObject& tile : m_Bricks)
		if (!tile.GetIsSolid() && !tile.GetDestroyed())
			return false;
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>>& tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (tileData[y][x] == 1) {
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.SetIsSolid(true);
				m_Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				m_Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("tile"), color));
			}
		}
	}
}
