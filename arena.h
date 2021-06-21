#pragma once

#include <GL/freeglut.h>
#include <array>
#include <string>

#include "utilities.h"

class Arena
{
public:
	Arena();
	~Arena() = default;

	GLdouble GetWidth()
	{
		return width;
	}
	
	GLdouble GetHeight()
	{
		return height;
	}

	GLdouble* GetPosition()
	{
		return position;
	}

	GLdouble* GetColor()
	{
		return color;
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
private:
	GLuint VBO{};
	GLuint VAO{};
	GLuint EBO{};

	GLdouble width{ 40 };
	GLdouble height{ 40 };

	GLdouble position[3]{ 0,20,-10 };
	GLdouble color[3]{ Utilities::GREY[0],Utilities::GREY[1],Utilities::GREY[2] };

	// matrices
	GLdouble translationMatrix[16]{};
	GLdouble rotationMatrix[16]{};

	std::string modelFileName{ "res/arena.obj" };

	std::vector<std::array<GLfloat, 3>> vertices{};
	std::vector<GLushort> indices{};
};