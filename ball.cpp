#include "ball.h"

Ball::Ball()
{
	GLdouble xVelocity = static_cast<GLdouble>(rand() % 5 + 1);
	xVelocity *= rand() % 2 == 0 ? -1 : 1;

	GLdouble yVelocity = 5;
	
	velocity[0] = xVelocity;
	velocity[0] = -4;
	velocity[1] = yVelocity;
}

void Ball::LoadModel()
{
	Utilities::LoadModel(modelFileName.c_str(), VBO, VAO, EBO, vertices, indices);
}

void Ball::Update(GLdouble deltaTime, GLdouble wallPosition[], GLdouble wallWidth, GLdouble wallHeight)
{
	GLdouble halfWallWidth = wallWidth / 2;
	GLdouble halfWallHeight = wallHeight / 2;

	if (position[0] - radius < wallPosition[0] - halfWallWidth)
	{
		position[0] = wallPosition[0] - halfWallWidth + radius;
		velocity[0] *= -1;
	}
	else if (position[0] + radius > wallPosition[0] + halfWallWidth)
	{
		position[0] = wallPosition[0] + halfWallWidth - radius;
		velocity[0] *= -1;
	}

	// did the ball fall outside the arena
	if (position[1] < wallPosition[1] - halfWallHeight - (radius * 2))
	{
		position[1] = wallPosition[1] - halfWallWidth - (radius * 2);
		
		velocity[0] = 0;
		velocity[1] = 0;
	}
	else if (position[1] + radius > wallPosition[1] + halfWallHeight)
	{
		position[1] = wallPosition[1] + halfWallHeight - radius;
		velocity[1] *= -1;
	}

	position[0] = position[0] + velocity[0] * deltaTime;
	position[1] = position[1] + velocity[1] * deltaTime;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(position[0], position[1], position[2]);

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
}

void Ball::Draw(GLdouble viewMatrix[])
{
	// switch to model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixd(viewMatrix);
	glMultMatrixd(modelMatrix);

	// bind VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// set color
	glColor3d(color[0], color[1], color[2]);

	// draw with index buffer
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, NULL);
}