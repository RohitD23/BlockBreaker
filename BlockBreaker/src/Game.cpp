#include "Game.h"
#include "GameObjects/Ball.h"
#include "Physics/Collison.h"
#include "ResourceManager/ResourceManager.h"
#include "Particle/Particle.h"
#include "TextRenderer/TextRenderer.h"

#include <irrKlang/irrKlang.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <sstream>

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

SpriteRenderer* Renderer;
GameObject* Player;
Ball* ball;
ParticleGenerator* Particles;
TextRenderer* Text;
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

Game::Game(unsigned int width, unsigned int height)
	:m_Width(width), m_Height(height), m_State(GameState::GAME_MENU), level(0), lives(3) {}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete ball;
	delete Particles;
	delete Text;
	SoundEngine->drop();
}

void Game::Init()
{
	//Load Shaders
	ResourceManager::LoadShader("res/Shaders/Shader.shader", "sprite");
	ResourceManager::LoadShader("res/Shaders/Particle.shader", "particle");

	//Load Textures
	ResourceManager::LoadTexture("res/Textures/Block.png", true, "block");
	ResourceManager::LoadTexture("res/Textures/BlockSquare.png", true, "tile");
	ResourceManager::LoadTexture("res/Textures/Player.png", true, "player");
	ResourceManager::LoadTexture("res/Textures/Ball.png", true, "ball");
	ResourceManager::LoadTexture("res/Textures/particle.png", true, "particle");
	ResourceManager::LoadTexture("res/Textures/powerup_increase.png", true, "puIncrease");
	ResourceManager::LoadTexture("res/Textures/powerup_passthrough.png", true, "puPassThrough");
	ResourceManager::LoadTexture("res/Textures/powerup_speed.png", true, "puSpeed");
	ResourceManager::LoadTexture("res/Textures/powerup_sticky.png", true, "puSticky");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Bind();
	ResourceManager::GetShader("sprite").SetUniform1i("u_image", 0);
	ResourceManager::GetShader("sprite").SetUniformMat4f("u_projection", projection);

	ResourceManager::GetShader("particle").Bind();
	ResourceManager::GetShader("particle").SetUniform1i("u_sprite", 0);
	ResourceManager::GetShader("particle").SetUniformMat4f("u_projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500
	);
	Text = new TextRenderer(projection);
	Text->Load("res/Fonts/OCRAEXT.TTF", 24);

	GameLevel one;
	one.Load("res/Levels/LevelOne.lvl.txt", m_Width, m_Height / 2);
	GameLevel two; 
	two.Load("res/Levels/LevelTwo.lvl.txt", m_Width, m_Height / 2);
	GameLevel three; 
	three.Load("res/Levels/LevelThree.lvl.txt", m_Width, m_Height / 2);
	GameLevel four; 
	four.Load("res/Levels/LevelFour.lvl.txt", m_Width, m_Height / 2);

	Levels.push_back(one);
	Levels.push_back(two);
	Levels.push_back(three);
	Levels.push_back(four);

	glm::vec2 playerPos = glm::vec2(m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, m_Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	ball = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));

	// audio
	SoundEngine->play2D("res/Audio/breakout.wav", true);
}

void Game::ProcessInput(float dt)
{
	if (m_State == GameState::GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (Keys[GLFW_KEY_A])
		{
			if (Player->GetPosition().x >= 5.0f) {
				Player->ChangePositionX(-velocity);
				if(ball->GetBallStuck())
					ball->ChangePositionX(-velocity);
			}
		}
		if (Keys[GLFW_KEY_D])
		{
			if (Player->GetPosition().x <= (m_Width - Player->GetSize().x - 5.0f)) {
				Player->ChangePositionX(velocity);
				if (ball->GetBallStuck())
					ball->ChangePositionX(velocity);
			}
		}
		if (Keys[GLFW_KEY_SPACE])
			ball->SetBallStuck(false);
	}
	else if(m_State == GameState::GAME_MENU) 
	{
		if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER])
		{
			m_State = GameState::GAME_ACTIVE;
			KeysProcessed[GLFW_KEY_ENTER] = true;
		}
		if (Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W])
		{
			level = (level + 1) % 4;
			KeysProcessed[GLFW_KEY_W] = true;
		}
		if (Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S])
		{
			if (level > 0)
				--level;
			else
				level = 3;
			KeysProcessed[GLFW_KEY_S] = true;
		}
	}

	if (m_State == GameState::GAME_WIN)
	{
		if (Keys[GLFW_KEY_ENTER])
		{
			KeysProcessed[GLFW_KEY_ENTER] = true;
			m_State = GameState::GAME_MENU;
		}
	}
}

void Game::Update(float dt)
{
	ball->Move(dt, m_Width);
	Particles->Update(dt, *ball, 2, glm::vec2(ball->GetRadius() / 2.0f));
	DoCollision();
	if (ball->GetPosition().y >= m_Height) // did ball reach bottom edge?
	{
		--lives;
		if (lives == 0)
		{
			ResetLevel();
			//m_State = GameState::GAME_MENU;
		}

		ResetPlayer();
	}
	UpdatePowerUps(dt);

	if (m_State == GameState::GAME_ACTIVE && Levels[level].isCompleted())
	{
		ResetLevel();
		ResetPlayer();
		m_State = GameState::GAME_WIN;
	}
}

void Game::Render()
{
	if(m_State == GameState::GAME_ACTIVE || m_State == GameState::GAME_MENU)
	{
		// draw level
		Levels[level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Particles->Draw();
		ball->Draw(*Renderer);
		for (PowerUp& powerUp : PowerUps) 
		{
			if (!powerUp.GetDestroyed())
				powerUp.Draw(*Renderer);
		}
		std::stringstream ss; ss << lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}

	if (m_State == GameState::GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, m_Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, m_Height / 2 + 20.0f, 0.75f);
	}

	if (m_State == GameState::GAME_WIN)
	{
		Text->RenderText(
			"You WON!!!", 320.0, m_Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
		);
		Text->RenderText(
			"Press ENTER to retry or ESC to quit", 130.0, m_Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
		);
	}
}

void Game::DoCollision()
{

	for (GameObject& box : Levels[level].GetBricks())
	{
		if (!box.GetDestroyed())
		{
			Collision collision = CheckCollision(*ball, box);
			if (std::get<0>(collision))
			{
				if (!box.GetIsSolid()) {
					box.SetDestroyed(true);
					SpawnPowerUps(box);
					SoundEngine->play2D("res/Audio/bleep.wav", false);
				}

				Direction dir = std::get<1>(collision);
				glm::vec2 diffVector = std::get<2>(collision);

				if (!(ball->GetPassThrough() && !box.GetIsSolid())) {
					if (dir == Direction::LEFT || dir == Direction::RIGHT)
					{
						ball->ChangeVelocityDirectionX();

						float penetration = ball->GetRadius() - std::abs(diffVector.x);

						if (dir == Direction::LEFT)
							ball->ChangePositionX(penetration);
						else
							ball->ChangePositionX(-penetration);
					}
					else
					{
						ball->ChangeVelocityDirectionY(); // reverse
						// relocate
						float penetration = ball->GetRadius() - std::abs(diffVector.y);
						if (dir == Direction::UP)
							ball->ChangePositionY(-penetration); // move up
						else
							ball->ChangePositionY(penetration); // move down
					}
				}
			}
		}
	}

	Collision result = CheckCollision(*ball, *Player);
	if (!ball->GetBallStuck() && std::get<0>(result))
	{
		// check where it hit the board, and change velocity
		float centerBoard = Player->GetPosition().x + Player->GetSize().x / 2.0f;
		float distance = (ball->GetPosition().x + ball->GetRadius()) - centerBoard;
		float percentage = distance / (Player->GetSize().x / 2.0f);

		// then move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = ball->GetVelocity();
		ball->SetVelocityX(INITIAL_BALL_VELOCITY.x * percentage * strength);
		ball->SetVelocityY(-1.0f * abs(ball->GetVelocity().y));
		ball->SetVelocity(glm::normalize(ball->GetVelocity()) * glm::length(oldVelocity));

		ball->SetBallStuck(ball->GetBallSticky());

		SoundEngine->play2D("res/Audio/bleep.wav", false);
	}

	for (PowerUp& powerUp : PowerUps)
	{
		if (!powerUp.GetDestroyed())
		{
			if (powerUp.GetPosition().y >= m_Height)
				powerUp.SetDestroyed(true);
			if (CheckCollision(*Player, powerUp))
			{	// collided with player, now activate powerup
				ActivatePowerUp(powerUp);
				powerUp.SetDestroyed(true);
				powerUp.SetActivated(true);
				SoundEngine->play2D("res/Audio/powerup.wav", false);
			}
		}
	}
}

void Game::ResetLevel()
{
	if (level == 0)
		Levels[0].Load("res/Levels/LevelOne.lvl.txt", m_Width, m_Height / 2);
	else if (level == 1)
		Levels[1].Load("res/Levels/LevelTwo.lvl.txt", m_Width, m_Height / 2);
	else if (level == 2)
		Levels[2].Load("res/Levels/LevelThree.lvl.txt", m_Width, m_Height / 2);
	else if (level == 3)
		Levels[3].Load("res/Levels/LevelFour.lvl.txt", m_Width, m_Height / 2);

	lives = 3;
	PowerUps.clear();
}

void Game::ResetPlayer()
{
	// reset player/ball stats
	Player->SetSize(PLAYER_SIZE);
	Player->SetPosition(glm::vec2(m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, m_Height - PLAYER_SIZE.y));
	ball->Reset(Player->GetPosition() + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(75)) // 1 in 75 chance
		PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.GetPosition(), ResourceManager::GetTexture("puSpeed")));
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 10.0f, block.GetPosition(), ResourceManager::GetTexture("puSticky")));
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.GetPosition(), ResourceManager::GetTexture("puPassThrough")));
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.GetPosition(), ResourceManager::GetTexture("puIncrease")));
}

void Game::UpdatePowerUps(float dt)
{
	for (PowerUp& powerUp : PowerUps) 
	{
		powerUp.Move(dt);

		if (powerUp.GetActivated())
		{
			powerUp.ChangeDuration(dt);

			if (powerUp.GetDuration() <= 0.0f)
			{
				powerUp.SetActivated(false);
				if (powerUp.GetType() == "sticky")
				{
					if (!IsOtherPowerUpActive("sticky"))
					{	// only reset if no other PowerUp of type sticky is active
						ball->SetBallSticky(false);
						Player->SetColor(glm::vec3(1.0f));
					}
				}
				else if (powerUp.GetType() == "pass-through")
				{
					if (!IsOtherPowerUpActive("pass-through"))
					{	// only reset if no other PowerUp of type pass-through is active
						ball->SetPassThrough(false);
						ball->SetColor(glm::vec3(1.0f));
					}
				}
			}
		}
	}

	PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(),
		[](const PowerUp& powerUp) { return powerUp.GetDestroyed() && !powerUp.GetActivated(); }
	), PowerUps.end());
}

void Game::ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.GetType() == "speed")
	{
		glm::vec2 v (ball->GetVelocity());
		v *= 1.2;
		ball->SetVelocity(v);
	}
	else if (powerUp.GetType() == "sticky")
	{
		ball->SetBallSticky(true);
		Player->SetColor(glm::vec3(1.0f, 0.5f, 1.0f));
	}
	else if (powerUp.GetType() == "pass-through")
	{
		ball->SetPassThrough(true);
		ball->SetColor(glm::vec3(1.0f, 0.5f, 0.5f));
	}
	else if (powerUp.GetType() == "pad-size-increase")
	{
		Player->ChangeSizeX(50.0f);
	}
}

bool Game::ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}

bool Game::IsOtherPowerUpActive(std::string type)
{
	// Check if another PowerUp of the same type is still active
	// in which case we don't disable its effect (yet)
	for (const PowerUp& powerUp : PowerUps)
	{
		if (powerUp.GetActivated())
		{
			if (powerUp.GetType() == type)
				return true;
		}
	}
	return false;
}
