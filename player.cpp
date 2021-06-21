#include "player.h"

Player::Player()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);
}

void Player::LoadModel()
{
	Utilities::LoadModel(modelFileName.c_str(), VBO, VAO, EBO, vertices, indices);
}

void Player::Update(GLdouble wallPosition[], GLdouble wallWidth, GLdouble wallHeight)
{
	GLdouble halfWidth = width / 2;
	GLdouble halfHeight = height / 2;
	GLdouble wallHalfWidth = wallWidth / 2;
	GLdouble wallHalfHeight = wallHeight / 2;

	if (position[0] + halfWidth >= wallPosition[0] + wallHalfWidth)
	{
		position[0] = wallPosition[0] + wallHalfWidth - halfWidth;
	}
	else if (position[0] - halfWidth <= wallPosition[0] - wallHalfWidth)
	{
		position[0] = wallPosition[0] - wallHalfWidth + halfWidth;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(position[0], position[1], position[2]);
	
	glGetDoublev(GL_MODELVIEW_MATRIX, translationMatrix);
}

void Player::Draw(GLdouble viewMatrix[])
{
	// switch to model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixd(viewMatrix);
	glMultMatrixd(rotationMatrix);
	glMultMatrixd(translationMatrix);

	// bind VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// set color
	glColor3d(color[0], color[1], color[2]);

	// draw with index buffer
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, NULL);
}

void Player::DetectCollision(GLdouble ballPosition[], GLdouble ballVelocity[], GLdouble radius)
{
	GLdouble halfWidth = width / 2;
	GLdouble halfHeight = height / 2;
	GLdouble p0[3]{}, p1[3]{}, p3[3]{};
	GLdouble normal0[3]{};
	GLdouble normal1[3]{};
	GLdouble t0{ std::numeric_limits<double>::max() },
		t1{ std::numeric_limits<double>::max() },
		t3{ std::numeric_limits<double>::max() },
		minA{}, minB{};
	GLdouble normalizedBallVelocity[3]{};
	Utilities::Normalize3(ballVelocity, normalizedBallVelocity);

	// is the ball within the x boundry
	if (ballPosition[0] + radius > position[0] - halfWidth
		&& ballPosition[0] - radius < position[0] + halfWidth)
	{
		// is the ball within the y boundry
		if (ballPosition[1] + radius > position[1] - halfHeight
			&& ballPosition[1] - radius < position[1] + halfHeight)
		{
			// Left, Right face hit?
			normal0[0] = -1; normal0[1] = 0; normal0[2] = 0; // left side normal
			normal1[0] = 1; normal1[1] = 0; normal1[2] = 0; // right side normal
			p0[0] = position[0] - halfWidth; p0[1] = position[1]; // left side center point on plane
			p1[0] = position[0] + halfWidth; p1[1] = position[1]; // right side center point on plane
			Utilities::IntersectPlane(normal0, p0, ballPosition, normalizedBallVelocity, t0, width, height);
			Utilities::IntersectPlane(normal1, p1, ballPosition, normalizedBallVelocity, t1, width, height);
			minA = std::min(abs(t0), abs(t1));

			// Top face hit?
			normal1[0] = 0; normal1[1] = 1; normal1[2] = 0; // top side normal
			p3[0] = position[0]; p3[1] = position[1] + halfHeight; // top side center point on plane
			Utilities::IntersectPlane(normal1, p3, ballPosition, normalizedBallVelocity, t3, width, height);
			minB = abs(t3);

			// is t (the abs time before impact) smaller on the L,R sides
			if (minA < minB)
			{
				ballVelocity[0] *= -1;
				if (t0 < t1)
				{
					ballPosition[0] = p0[0] - radius;
				}
				else
				{
					ballPosition[0] = p1[0] + radius;
				}
				return;
			}
			else if (minB < minA) // is t (the abs time before impact) smaller on the T,B sides
			{
				ballVelocity[1] *= -1;
				ballPosition[1] = p3[1] + radius;
				return;
			}
			else
			{
				return;
			}
		}
	}
}