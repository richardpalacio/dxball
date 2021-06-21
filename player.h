#pragma once

#include <GL/freeglut.h>
#include <array>
#include <string>

#include "utilities.h"

class Player
{
public:
	Player();
	~Player() = default;

	GLdouble* GetPosition()
	{
		return position;
	}

	GLdouble* GetColor()
	{
		return color;
	}

	GLdouble* GetModelMatrix()
	{
		return translationMatrix;
	}

	void SetModelMatrix(GLdouble mat[16])
	{
		memcpy(translationMatrix, mat, 16 * sizeof(mat));
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
	void Update(GLdouble wallPosition[], GLdouble wallWidth, GLdouble wallHeight);
	void Draw(GLdouble viewMatrix[]);
	void DetectCollision(GLdouble ballPosition[], GLdouble ballVelocity[], GLdouble radius);
private:
	GLuint VBO{};
	GLuint VAO{};
	GLuint EBO{};

	GLdouble width{ 10 };
	GLdouble height{ 2 };

	GLdouble position[3]{ 0,1,0 };
	GLdouble color[3]{ 0,1,0 };

	// matrices
	GLdouble rotationMatrix[16]{ 0 };
	GLdouble translationMatrix[16]{ 0 };

	std::string modelFileName{ "res/paddle.obj" };

	std::vector<std::array<GLfloat, 3>> vertices{};
	std::vector<GLushort> indices{};
};