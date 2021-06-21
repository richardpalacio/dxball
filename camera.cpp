#include "camera.h"

Camera::Camera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, rotationMatrix);
}

void Camera::Update()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// look: position + (nAxis * -5);
	GLdouble lookAtPosition[3]{};
	lookAtPosition[0] = position[0] + (nAxis[0] * -5);
	lookAtPosition[1] = position[1] + (nAxis[1] * -5);
	lookAtPosition[2] = position[2] + (nAxis[2] * -5);

	gluLookAt(
		position[0], position[1], position[2],
		lookAtPosition[0], lookAtPosition[1], lookAtPosition[2],
		vAxis[0], vAxis[1], vAxis[2]
	);

	glMultMatrixd(rotationMatrix);

	// save matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, viewMatrix);
}
