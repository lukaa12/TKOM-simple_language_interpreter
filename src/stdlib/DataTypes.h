#ifndef DATATYPES_H
#define DATATYPES_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace tkom {
	namespace lib {

		class Color
		{
		public:
			glm::vec3 color;

		Color(int red, int green, int blue): color(red/250.0f, green / 250.0f, blue / 250.0f)
		{}

		Color(): color(0.5f, 0.5f, 0.5f)
		{}

		};



		class Graphic
		{
		public:
			Color color;
			bool fill;
			
			glm::mat3 transform;

			GLuint height;
			GLuint width;
			std::vector<Graphic> children;
			std::vector<float> vertices;
			std::vector<GLuint> indices;

			GLuint VBO, EBO, VAO;
			GLenum drawingMode;

			void start();
			void render(GLuint shader, glm::mat3 transform);
			void destroy();

			Graphic() : height(600), width(800), color(Color()), drawingMode(GL_TRIANGLES), fill(true)
			{}
			
		};

		

	}
}

#endif // !DATATYPES_H

