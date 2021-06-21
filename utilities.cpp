#include "utilities.h"

std::vector<const GLdouble*> Utilities::BlockColors{
	Utilities::RED, Utilities::PINK, Utilities::PURPLE, Utilities::DEEP_PURPLE, Utilities::INDIGO, Utilities::BLUE, Utilities::LIGHT_BLUE, Utilities::CYAN, Utilities::TEAL, Utilities::ORANGE
};

std::vector<const GLdouble*> Utilities::BlockPositions{
	Utilities::ONE, Utilities::TWO,
	Utilities::THREE, Utilities::FOUR,
	Utilities::FIVE, Utilities::SIX,
	Utilities::SEVEN, Utilities::EIGHT,
	Utilities::NINE, Utilities::TEN
};

void Utilities::LoadModel(const char* filename,
	GLuint& VBO, GLuint& VAO, GLuint& EBO,
	std::vector<std::array<GLfloat, 3>>& vertices, std::vector<GLushort>& indices)
{
	// load object file
	LoadObjFile(filename, vertices, indices);

	// create buffers (vertex buffer object, vertex attribute object, element buffer object)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(GLfloat), vertices.data()->data(), GL_STATIC_DRAW);

	// bind EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Utilities::LoadObjFile(const char* filename, std::vector<std::array<GLfloat, 3>>& vertices, std::vector<GLushort>& indices)
{
	std::ifstream in(filename, std::ios::in);
	if (!in)
	{
		std::cerr << "Cannot open " << filename << std::endl; exit(1);
	}

	std::string line;
	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			std::array<GLfloat, 3> v{};
			s >> v[0];
			s >> v[1];
			s >> v[2];
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(2));
			std::string noop;
			GLushort a, b, c;

			// skip over the texture and normal coordinates
			s >> a;
			std::getline(s, noop, ' ');

			s >> b;
			std::getline(s, noop, ' ');

			s >> c;
			std::getline(s, noop, ' ');
			--a; --b; --c;

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}
	}
}