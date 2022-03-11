#pragma once
#include <vector>
#include "GameLevel.h"
#include "PowerUp.h"

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	Game(unsigned int width, unsigned int height);
	~Game();

	//Initialize game state(load all shaders/textures/levels)
	void Init();
	//game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	void SetKey(int key, bool state) { Keys[key] = state; }
	void SetKeyProcessed(int key, bool state) { KeysProcessed[key] = state; }

private:
	GameState m_State;
	bool Keys[1024];
	bool KeysProcessed[1024];
	unsigned int m_Width, m_Height;
	unsigned int level, lives;
	std::vector<GameLevel> Levels;
	std::vector<PowerUp> PowerUps;

private:
	void DoCollision();

	void ResetLevel();
	void ResetPlayer();
	
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);
	void ActivatePowerUp(PowerUp& powerUp);
	bool ShouldSpawn(unsigned int chance);
	bool IsOtherPowerUpActive(std::string type);
};

