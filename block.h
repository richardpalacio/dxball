#pragma once

#include <GL/freeglut.h>
#include <array>
#include <string>
#include <utility>
#include <limits>

#include "utilities.h"

class Block
{
public:
	Block();
	~Block() = default;

	GLdouble* GetPosition()
	{
		return position;
	}

	void SetPosition(const GLdouble pos[])
	{
		position[0] = pos[0];
		position[1] = pos[1];
		position[2] = pos[2];

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslated(position[0], position[1], position[2]);
		glGetDoublev(GL_MODELVIEW_MATRIX, translationMatrix);
	}

	GLdouble* GetColor()
	{
		return color;
	}
	
	void SetColor(const GLdouble* col)
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
	}

	std::string GetModelFileName()
	{
		return modelFileName;
	}

	GLuint GetVBO()
	{
		return VBO;
	}

	GLuint GetVAO()
	{
		return VAO;
	}

	GLuint GetEBO()
	{
		return EBO;
	}

	std::vector<std::array<GLfloat, 3>>& GetVertices()
	{
		return vertices;
	}

	std::vector<GLushort>& GetIndices()
	{
		return indices;
	}

	void LoadModel();
	void Update();
	void Draw(GLdouble viewMatrix[]);
	bool DetectCollision(GLdouble ballPosition[], GLdouble ballVelocity[], GLdouble radius);
private:
	GLuint VBO{};
	GLuint VAO{};
	GLuint EBO{};

	GLdouble width{ 4 };
	GLdouble height{ 2 };

	GLdouble position[3]{ 0 };
	GLdouble color[3]{ 0,0,1 };
	
	// matrices
	GLdouble rotationMatrix[16]{ 0 };
	GLdouble translationMatrix[16]{ 0 };

	std::string modelFileName{ "res/block.obj" };

	std::vector<std::array<GLfloat, 3>> vertices{};
	std::vector<GLushort> indices{};
};