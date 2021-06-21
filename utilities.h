#pragma once

#include <GL/freeglut.h>

#include <vector>
#include <array>
#include <limits>

class Utilities
{
public:
	static constexpr GLdouble RED[3]{ 0.95686274509803921568627,0.2627450980392156862745098,0.21176470588235294117647058 };
	static constexpr GLdouble PINK[3]{ 0.913725490196078431372549,0.117647058823529411764705882,0.38823529411764705882352941 };
	static constexpr GLdouble PURPLE[3]{ 0.611764705882352941176470588,0.15294117647058823529411764,0.6901960784313725490196078431 };
	static constexpr GLdouble DEEP_PURPLE[3]{ 0.4039215686274509803921568627451,0.22745098039215686274509803921569,0.71764705882352941176470588235294 };
	static constexpr GLdouble INDIGO[3]{ 0.24705882352941176470588235294118,0.31764705882352941176470588235294,0.70980392156862745098039215686275 };
	static constexpr GLdouble BLUE[3]{ 0.12941176470588235294117647058824,0.58823529411764705882352941176471,0.95294117647058823529411764705882 };
	static constexpr GLdouble LIGHT_BLUE[3]{ 0.01176470588235294117647058823529,0.66274509803921568627450980392157,0.95686274509803921568627450980392 };
	static constexpr GLdouble CYAN[3]{ 0,0.73725490196078431372549019607843,0.83137254901960784313725490196078 };
	static constexpr GLdouble TEAL[3]{ 0,0.58823529411764705882352941176471,0.53333333333333333333333333333333 };
	static constexpr GLdouble ORANGE[3]{ 1,0.5960784313725490196078431372549,0 };

	static constexpr GLdouble GREY[3]{ 0.4588235294117,0.4588235294117,0.4588235294117 };
	static constexpr GLdouble YELLOW[3]{ 0,0,0 };

	static constexpr GLdouble ONE[3]{ -8,39,0 };
	static constexpr GLdouble TWO[3]{ -4,39,0 };
	static constexpr GLdouble THREE[3]{ 0,39,0 };
	static constexpr GLdouble FOUR[3]{ 4,39,0 };
	static constexpr GLdouble FIVE[3]{ 8,39,0 };
	static constexpr GLdouble SIX[3]{ -8,37,0 };
	static constexpr GLdouble SEVEN[3]{ -4,37,0 };
	static constexpr GLdouble EIGHT[3]{ 0,37,0 };
	static constexpr GLdouble NINE[3]{ 4,37,0 };
	static constexpr GLdouble TEN[3]{ 8,37,0 };

	static constexpr GLdouble PI = 3.1415926535897932384626433832795;

	static constexpr bool WIREFRAME = false;

	static void LoadModel(const char* filename,
		GLuint& VBO, GLuint& VAO, GLuint& EBO,
		std::vector<std::array<GLfloat, 3>>& vertices, std::vector<GLushort>& indices);

	static std::vector<const GLdouble*> BlockColors;
	static std::vector<const GLdouble*> BlockPositions;

	// PI = 180; PI / 180 = 1
	static constexpr GLdouble DegreeToRadians(GLdouble degree)
	{
		return degree / 180 * PI;
	}

	// ||u|| = u / sqrt(u.x^2 + u.y^2 + u.z^2)
	static void Normalize3(const GLdouble v[3], GLdouble normalizedValue[3])
	{
		double s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		normalizedValue[0] = v[0] / s;
		normalizedValue[1] = v[1] / s;
		normalizedValue[2] = v[2] / s;
	}

	// v dot u = v.x * v.x + u.y * v.y + u.z * v.z
	static constexpr GLdouble Dot3(const GLdouble v[3], const GLdouble u[3])
	{
		return v[0] * u[0] + v[1] * u[1] + v[2] * u[2];
	}

	/*
	v0 = ray origin point
	v = normalized ray direction vector
	p0 = a position on the plane
	normal = normalized normal of plane
	
	eq:
	p1 = a desired position on plane
	1) (p1 - p0) dot n = 0
	2) p1 = v0 + t * v;
	1_1) (v0 + (t * v) - p0) dot n = 0
	1_2) (v0 - p0) dot n + (t * v dot n) = 0
	1_3) t * (v dot n) = -(v0 - p0) dot n
	1_4) t = (p0 - v0) dot n / (v dot n)
	*/
	static bool IntersectPlane(const GLdouble normal[3], const GLdouble p0[3], const GLdouble v0[3], const GLdouble v[3],
		GLdouble& t, const GLdouble width, const GLdouble height)
	{
		GLdouble p1[3]{};
		GLdouble pTemp[3]{};
		GLdouble denominator{};
		GLdouble halfWidth = width / 2;
		GLdouble halfHeight = height / 2;

		denominator = Utilities::Dot3(normal, v);
		if (abs(denominator) > 1e-6)
		{
			p1[0] = p0[0] - v0[0];
			p1[1] = p0[1] - v0[1];
			p1[2] = p0[2] - v0[2];
			t = Utilities::Dot3(normal, p1) / denominator;

			// check that the bound is within the box boundry
			p1[0] = v0[0] + (t * v[0]);
			p1[1] = v0[1] + (t * v[1]);
			p1[2] = v0[2] + (t * v[2]);

			// distance from center point of plane to p at t
			pTemp[0] = p1[0] - p0[0];
			pTemp[1] = p1[1] - p0[1];
			pTemp[2] = p1[2] - p0[2];

			if (abs(pTemp[0]) < halfWidth && abs(pTemp[1]) < halfHeight)
			{
				if (t >= 0) {
					return true;
				}
			}
			else
			{
				t = std::numeric_limits<double>::max();
			}
		}

		return false;
	}
private:
	static void LoadObjFile(const char* filename, std::vector<std::array<GLfloat, 3>>& vertices, std::vector<GLushort>& indices);
};