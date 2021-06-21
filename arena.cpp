#include "arena.h"

Arena::Arena()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);

	glLoadIdentity();
	glTranslated(position[0], position[1], position[2]);
	glGetDoublev(GL_MODELVIEW_MATRIX, translationMatrix);
}

void Arena::LoadModel()
{
	Utilities::LoadModel(modelFileName.c_str(), VBO, VAO, EBO, vertices, indices);
}

void Arena::Update()
{
}

void Arena::Draw(GLdouble viewMatrix[])
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixd(viewMatrix);
	glMultMatrixd(translationMatrix);
	glMultMatrixd(rotationMatrix);
	
	// bind VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// set color
	glColor3d(color[0], color[1], color[2]);

	glDisable(GL_CULL_FACE);

	// draw with index buffer
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, NULL);

	glEnable(GL_CULL_FACE);
}