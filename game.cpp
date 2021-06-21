#include "game.h"

void Game::Initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));
	deltaTime = 0;
	currentState = GameState::MENU;

	camera = Camera{};
	arena = Arena{};
	player = Player{};
	ball = Ball{};
	blocks = std::vector<Block>{ 10 };

	// load models
	std::vector<unsigned int> colorIndexArray{ 0,1,2,3,4,5,6,7,8,9 };
	std::random_shuffle(colorIndexArray.begin(), colorIndexArray.end());

	std::vector<unsigned int> positionIndexArray{ 0,1,2,3,4,5,6,7,8,9 };
	std::random_shuffle(positionIndexArray.begin(), positionIndexArray.end());

	arena.LoadModel();
	player.LoadModel();
	ball.LoadModel();
	for (auto& item : blocks)
	{
		unsigned int colorIndex = colorIndexArray.back();
		unsigned int positionIndex = positionIndexArray.back();

		item.LoadModel();
		item.SetColor(Utilities::BlockColors[colorIndex]);
		item.SetPosition(Utilities::BlockPositions[positionIndex]);

		colorIndexArray.pop_back();
		positionIndexArray.pop_back();
	}

	currentState = GameState::MENU;
}

void Game::Update()
{
	if (currentState == GameState::MENU || currentState == GameState::PLAYING)
	{
		// update camera parameters
		camera.Update();

		// update all objects and check for collisions
		arena.Update();
		ball.Update(deltaTime, arena.GetPosition(), arena.GetWidth(), arena.GetHeight());
		player.Update(arena.GetPosition(), arena.GetWidth(), arena.GetHeight());
		player.DetectCollision(ball.GetPosition(), ball.GetVelocity(), ball.GetRadius());

		std::vector<Block>::iterator it = blocks.begin();
		while (it != blocks.end())
		{
			// update blocks
			it->Update();

			// check for collisions
			bool collision = it->DetectCollision(ball.GetPosition(), ball.GetVelocity(), ball.GetRadius());
			if (collision) {
				it = blocks.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		if (blocks.size() == 0 || ball.GetVelocity()[1] == 0)
		{
			EndGame();
		}
	}
	else if (currentState == GameState::OVER)
	{
		EndGame();
	}
	else if (currentState == GameState::PAUSED)
	{
		Pause();
	}

	// draw all objects
	arena.Draw(camera.GetViewMatrix());
	player.Draw(camera.GetViewMatrix());
	ball.Draw(camera.GetViewMatrix());
	for (auto& item : blocks)
	{
		item.Draw(camera.GetViewMatrix());
	}
}

void Game::EndGame()
{
	currentState = GameState::OVER;

	std::string outputText = "Sorry, You lost";
	const char* outputCharBuffer;
	deltaTime = 0;

	if (blocks.size() == 0)
	{
		outputText = "You Won";
	}

	outputCharBuffer = outputText.c_str();

	glWindowPos2i(5, 5);
	while (*outputCharBuffer)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *outputCharBuffer++);
	}
}

void Game::Pause()
{
	currentState = GameState::PAUSED;
	deltaTime = 0;
	std::string outputText = "Paused";
	const char* outputCharBuffer;

	outputCharBuffer = outputText.c_str();

	glWindowPos2i(5, 5);
	while (*outputCharBuffer)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *outputCharBuffer++);
	}
}

void Game::UnPause()
{
	currentState = GameState::PLAYING;
	deltaTime = 0.1;
}

void Game::Key(unsigned char ch, int x, int y)
{
	bool changedSettings = true;
	GLdouble* position = camera.GetPosition();
	GLdouble* lookAxis = camera.GetLookAxis();
	GLdouble* rotationMatrix = camera.GetRotationMatrix();
	GLdouble arenaHalfWidth = arena.GetWidth() / 2;
	GLdouble arenaHalfHeight = arena.GetHeight() / 2;

	switch (ch)
	{
	case 27: // escape
		glutLeaveMainLoop();
		break;
	case 120: // x
		position[0] = position[0] + (lookAxis[0] * 100);
		position[1] = position[1] + (lookAxis[1] * 100);
		position[2] = position[2] + (lookAxis[2] * 100);
		break;
	case 122: // z
		position[0] = position[0] + (lookAxis[0] * -100);
		position[1] = position[1] + (lookAxis[1] * -100);
		position[2] = position[2] + (lookAxis[2] * -100);
		break;
	case 97: // a
		position[0] -= 1;
		break;
	case 100: // d
		position[0] += 1;
		break;
	case 119: // w
		position[1] -= 1;
		break;
	case 115: // s
		position[1] += 1;
		break;
	case 112: // p
		Pause();
		break;
	case 32: // space
		UnPause();
		break;
	case 114: // r
		Initialize();
		currentState = GameState::MENU;
		break;
	case 93: // ]
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(rotationMatrix);
		glRotated(-0.5, 0, 1, 0);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);

		// update camera parameters
		camera.Update();
		break;
	case 91: // [
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(rotationMatrix);
		glRotated(0.5, 0, 1, 0);
		glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);

		// update camera parameters
		camera.Update();
		break;
	case 39: // "
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(rotationMatrix);
		
		glTranslated(0, arenaHalfHeight, 0);
		glRotated(0.5, 1, 0, 0);
		glTranslated(0, -arenaHalfHeight, 0);

		glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);

		// update camera parameters
		camera.Update();
		break;
	case 59: // :
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(rotationMatrix);
		
		glTranslated(0, arenaHalfHeight, 0);
		glRotated(-0.5, 1, 0, 0);
		glTranslated(0, -arenaHalfHeight, 0);
		
		glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);

		// update camera parameters
		camera.Update();
		break;
	default:
		changedSettings = false;
		break;
	}

	if (changedSettings)
	{
		glutPostRedisplay();
	}
}

void Game::SpecialKey(int key, int x, int y)
{
	GLdouble* playerPosition = player.GetPosition();
	GLdouble* ballPosition = ball.GetPosition();
	GLdouble* rightAxis = camera.GetRightAxis();
	GLdouble* upAxis = camera.GetUpAxis();
	GLdouble* lookAxis = camera.GetLookAxis();
	GLdouble* rotationMatrix = camera.GetRotationMatrix();
	GLdouble UVNMatrix[16]{};

	// initialize
	camera.GetUVNMatrix(UVNMatrix);

	// move paddle right
	if (key == GLUT_KEY_RIGHT)
	{
		if (currentState == GameState::MENU)
		{
			playerPosition[0] += 3;
			player.Update(arena.GetPosition(), arena.GetWidth(), arena.GetHeight());
			ballPosition[0] = playerPosition[0];
		}
		else if (currentState == GameState::PLAYING)
		{
			playerPosition[0] += 3;
		}
	}
	else if (key == GLUT_KEY_LEFT) // move paddle left
	{
		if (currentState == GameState::MENU)
		{
			playerPosition[0] -= 3;
			player.Update(arena.GetPosition(), arena.GetWidth(), arena.GetHeight());
			ballPosition[0] = playerPosition[0];
		}
		else if (currentState == GameState::PLAYING)
		{
			playerPosition[0] -= 3;
		}
	}
	else if (key == GLUT_KEY_PAGE_UP)
	{
		// rotate the up and look camera axis vectors with the right axis as the rotation axis
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glLoadMatrixd(UVNMatrix);
		glRotated(.05, rightAxis[0], rightAxis[1], rightAxis[2]);
		glGetDoublev(GL_MODELVIEW_MATRIX, UVNMatrix);

		// extract up and look vectors
		upAxis[0] = UVNMatrix[4];
		upAxis[1] = UVNMatrix[5];
		upAxis[2] = UVNMatrix[6];
		Utilities::Normalize3(upAxis, upAxis);

		lookAxis[0] = UVNMatrix[8];
		lookAxis[1] = UVNMatrix[9];
		lookAxis[2] = UVNMatrix[10];
		Utilities::Normalize3(lookAxis, lookAxis);
	}
	else if (key == GLUT_KEY_PAGE_DOWN)
	{
		// rotate the up and look camera axis vectors with the right axis as the rotation axis
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glLoadMatrixd(UVNMatrix);
		glRotated(-.05, rightAxis[0], rightAxis[1], rightAxis[2]);
		glGetDoublev(GL_MODELVIEW_MATRIX, UVNMatrix);

		// extract up and look vectors
		upAxis[0] = UVNMatrix[4];
		upAxis[1] = UVNMatrix[5];
		upAxis[2] = UVNMatrix[6];
		Utilities::Normalize3(upAxis, upAxis);

		lookAxis[0] = UVNMatrix[8];
		lookAxis[1] = UVNMatrix[9];
		lookAxis[2] = UVNMatrix[10];
		Utilities::Normalize3(lookAxis, lookAxis);
	}
	else if (key == GLUT_KEY_HOME)
	{
		// rotate the up and look camera axis vectors with the right axis as the rotation axis
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glLoadMatrixd(UVNMatrix);
		glRotated(.05, upAxis[0], upAxis[1], upAxis[2]);
		glGetDoublev(GL_MODELVIEW_MATRIX, UVNMatrix);

		// extract up and look vectors
		rightAxis[0] = UVNMatrix[0];
		rightAxis[1] = UVNMatrix[1];
		rightAxis[2] = UVNMatrix[2];

		lookAxis[0] = UVNMatrix[8];
		lookAxis[1] = UVNMatrix[9];
		lookAxis[2] = UVNMatrix[10];
	}
	else if (key == GLUT_KEY_END)
	{
		// rotate the up and look camera axis vectors with the right axis as the rotation axis
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glLoadMatrixd(UVNMatrix);
		glRotated(-.05, upAxis[0], upAxis[1], upAxis[2]);
		glGetDoublev(GL_MODELVIEW_MATRIX, UVNMatrix);

		// extract up and look vectors
		rightAxis[0] = UVNMatrix[0];
		rightAxis[1] = UVNMatrix[1];
		rightAxis[2] = UVNMatrix[2];

		lookAxis[0] = UVNMatrix[8];
		lookAxis[1] = UVNMatrix[9];
		lookAxis[2] = UVNMatrix[10];
	}

	glutPostRedisplay();
}