#pragma once

#include "utilities.h"

class Camera
{
public:
	Camera();
	~Camera() = default;

	GLdouble* GetRotationMatrix()
	{
		return rotationMatrix;
	}

	GLdouble* GetPosition()
	{
		return position;
	}

	void SetPosition(GLdouble pos[])
	{
		position[0] = pos[0];
		position[1] = pos[1];
		position[2] = pos[2];
	}

	GLdouble* GetRightAxis()
	{
		return uAxis;
	}

	GLdouble* GetUpAxis()
	{
		return vAxis;
	}

	GLdouble* GetLookAxis()
	{
		return nAxis;
	}

	void SetRightAxis(GLdouble* v)
	{
		uAxis[0] = v[0];
		uAxis[1] = v[1];
		uAxis[2] = v[2];
	}

	void SetUpAxis(GLdouble* v)
	{
		vAxis[0] = v[0];
		vAxis[1] = v[1];
		vAxis[2] = v[2];
	}

	void SetLookAxis(GLdouble* v)
	{
		nAxis[0] = v[0];
		nAxis[1] = v[1];
		nAxis[2] = v[2];
	}

	GLdouble* GetViewMatrix()
	{
		return viewMatrix;
	}
	
	void SetViewMatrix(GLdouble viewMat[16])
	{
		memcpy(viewMatrix, viewMat, 16 * sizeof(viewMat));
	}

	void Update();

	void GetUVNMatrix(GLdouble mat[16])
	{
		mat[0] = uAxis[0];
		mat[1] = uAxis[1];
		mat[2] = uAxis[2];
		mat[3] = 0;
		mat[4] = vAxis[0];
		mat[5] = vAxis[1];
		mat[6] = vAxis[2];
		mat[7] = 0;
		mat[8] = nAxis[0];
		mat[9] = nAxis[1];
		mat[10] = nAxis[2];
		mat[11] = 0;
		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 0;
	}
private:
	GLdouble position[3]{ 0,20,4000 };
	GLdouble uAxis[3]{ 1,0,0 };
	GLdouble vAxis[3]{ 0,1,0 };
	GLdouble nAxis[3]{ 0,0,1 };

	GLdouble modelMatrix[16]{};
	GLdouble rotationMatrix[16]{};
	GLdouble viewMatrix[16]{};
};