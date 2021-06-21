#pragma once

#include "camera.h"
#include "arena.h"
#include "player.h"
#include "ball.h"
#include "block.h"

class Game
{
public:
	Game() = default;
	~Game() = default;

	void Initialize();
	void EndGame();
	void Pause();
	void UnPause();
	void Update();
	void Key(unsigned char ch, int x, int y);
	void SpecialKey(int key, int x, int y);
private:
	enum class GameState
	{
		MENU,
		PLAYING,
		PAUSED,
		OVER
	};
	GameState currentState{ GameState::MENU };

	Camera camera{};
	Arena arena{};
	Player player{};
	Ball ball{};
	std::vector<Block> blocks{ 10 };

	GLdouble deltaTime = 0;
};