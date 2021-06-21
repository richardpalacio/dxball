#include <GL/glew.h>
#include <GL/freeglut.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

#include "game.h"
#include "camera.h"
#include "player.h"
#include "block.h"
#include "arena.h"
#include "ball.h"
#include "utilities.h"

Game game{};

/*
report GL errors to console
*/
void checkError(const wchar_t* functionName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		wchar_t c_str[64]{};
		swprintf_s(c_str, 64, L"GL error 0x%X detected in %s\n", error, functionName);
		OutputDebugString(c_str);
	}
}

/*
display callback function
*/
void display()
{
	const double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game.Update();
	
	glutSwapBuffers();
}

/*
handle ascii character keys
*/
void key(unsigned char ch, int x, int y)
{
	game.Key(ch, x, y);
}

/*
handle arrow keys
*/
void special(int key, int x, int y)
{
	game.SpecialKey(key, x, y);
}

/*
update perspective projection
*/
void resize(int width, int height)
{
	GLdouble aspectRatio = static_cast<GLdouble>(width) / static_cast<GLdouble>(height);
	GLdouble fov = Utilities::PI / 4;

	// setup viewport
	glViewport(0, 0, width, height);

	// switch to projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fov, aspectRatio, 1.0, 10000.0);
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	GLenum glewInitResult;
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	// init glut
	glutInit(&argc, argv);
	glutInitDisplayString("DX Ball");

	// double buffer, depth buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow("DXBall");

	// callbacks
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutSpecialFunc(special);

	// check for error on init
	checkError(L"init");

	// initialize glew
	glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK)
	{
		OutputDebugString(L"Glew not initialized");
		return 1;
	}

	// start a new game
	game.Initialize();

	// wireframe
	if (Utilities::WIREFRAME == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_SCISSOR_TEST);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
	}

	// start game loop
	glutMainLoop();

#ifdef _MSC_VER
	/* DUMP MEMORY LEAK INFORMATION */
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}