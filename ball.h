#pragma once

#include <GL/freeglut.h>
#include <array>
#include <string>
#include <cstdlib>

#include "utilities.h"

class Ball
{
public:
	Ball();
	~Ball() = default;

	GLdouble GetRadius()
	{
		return radius;
	}

	GLdouble* GetPosition()
	{
		return position;
	}

	GLdouble* GetVelocity()
	{
		return velocity;
	}

	GLdouble* GetColor()
	{
		return color;
	}

	GLdouble* GetModelMatrix()
	{
		return modelMatrix;
	}

	void SetModelMatrix(GLdouble mat[16])
	{
		memcpy(modelMatrix, mat, 16 * sizeof(mat));
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
	void Update(GLdouble deltaTime, GLdouble wallPosition[], GLdouble wallWidth, GLdouble wallHeight);
	void Draw(GLdouble viewMatrix[]);
private:
	GLuint VBO{};
	GLuint VAO{};
	GLuint EBO{};

	GLdouble radius{ 0.5 };

	GLdouble position[3]{ 0,3,0 };
	GLdouble color[3]{ 1,0,0 };
	GLdouble modelMatrix[16]{ 0 };

	GLdouble velocity[3]{ 3,5,0 };

	std::string modelFileName{ "res/ball.obj" };

	std::vector<std::array<GLfloat, 3>> vertices{};
	std::vector<GLushort> indices{};
};
