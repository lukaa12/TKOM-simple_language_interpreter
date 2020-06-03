#include "DataTypes.h"

using namespace tkom;
using namespace lib;

void Graphic::start()
{
	if (vertices.size() != 0)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	for (auto it = children.begin(); it != children.end(); ++it)
		it->start();
}

void Graphic::render(GLuint shader, glm::mat3 trans)
{
	if (fill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	trans = trans * this->transform;
	unsigned int transformLoc = glGetUniformLocation(shader, "transform"), colorLoc = glGetUniformLocation(shader, "inColor");
	glUniformMatrix3fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color.color));

	if (vertices.size() != 0)
	{
		glBindVertexArray(VAO);
		glDrawElements(drawingMode, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	for (auto it = children.begin(); it != children.end(); ++it)
		it->render(shader, trans);
}

void Graphic::destroy()
{
	if (vertices.size() != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	
	for (auto it = children.begin(); it != children.end(); ++it)
		it->destroy();
}
