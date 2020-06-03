#include "Functions.h"
#include "../Error.h"

using namespace tkom;
using namespace lib;

GLuint compileShaders();
GLFWwindow* initWindow(GLuint w, GLuint h);

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"uniform mat3 transform;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(transform * vec3(position, 1.0), 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec3 inColor;\n"
"void main()\n"
"{\n"
"color = vec4(inColor, 1.0f);\n"
"}\n\0";

int tkom::lib::print(std::string in)
{
	std::cout << in << std::endl;
	return 0;
}

Graphic tkom::lib::blank(int w, int h)
{
	Graphic blank;
	blank.width = w;
	blank.height = h;
	blank.color.color = glm::vec3(1.0f, 1.0f, 1.0f);
	blank.transform = glm::mat3(1.0f);
	blank.transform = glm::scale(blank.transform, glm::vec2(2.0f / w, 2.0f / h));

	return blank;
}

Graphic tkom::lib::triangle(int h, int w)
{
	Graphic triangle;
	triangle.transform = glm::mat3(1.0f);
	triangle.vertices = {
		   0.0f,  h/2.0f,
		-w/2.0f, -h/2.0f,
		 w/2.0f, -h/2.0f,
	};
	triangle.indices = {
		0, 1, 2
	};

	return triangle;
}

Graphic tkom::lib::rectangularTriangle(int a, int b)
{
	Graphic triangle;
	triangle.transform = glm::mat3(1.0f);
	triangle.vertices = {
		-a / 2.0f,  b / 2.0f,
		-a / 2.0f, -b / 2.0f,
		 a / 2.0f, -b / 2.0f,
	};
	triangle.indices = {
		0, 1, 2
	};

	return triangle;
}

Graphic tkom::lib::rectangle(int a, int b)
{
	Graphic rect;
	rect.transform = glm::mat3(1.0f);
	rect.vertices = {
		 a / 2.0f,  b / 2.0f,
		-a / 2.0f,  b / 2.0f,
		-a / 2.0f, -b / 2.0f,
		 a / 2.0f, -b / 2.0f,
	};
	rect.indices = {
		0, 1, 2,
		2, 3, 0
	};

	return rect;
}

Graphic tkom::lib::circle(int r)
{
	Graphic circle;
	circle.transform = glm::mat3(1.0f);
	circle.vertices = { 0.0f, 0.0f };
	for (int i = 0; i != 36; ++i)
	{
		circle.vertices.push_back(glm::cos(glm::radians(i * 10.0f))*r);
		circle.vertices.push_back(glm::sin(glm::radians(i * 10.0f))*r);
		circle.indices.push_back(0);
		circle.indices.push_back(i + 1);
		circle.indices.push_back(i + 2 > 36 ? 1 : i + 2);
	}
	return circle;
}

Graphic tkom::lib::line(int len)
{
	return rectangle(len, 20);
}

Graphic tkom::lib::add(Graphic parent, Graphic child)
{
	parent.children.push_back(child);
	return parent;
}

Graphic tkom::lib::translate(Graphic object, int x, int y)
{
	object.transform = glm::translate(object.transform, glm::vec2(static_cast<float>(x), static_cast<float>(y)));
	return object;
}

Graphic tkom::lib::scale(Graphic object, int x, int y)
{
	object.transform = glm::scale(object.transform, glm::vec2(x/100.0f, y/100.0f));
	return object;
}

Graphic tkom::lib::rotate(Graphic object, int x)
{
	object.transform = glm::rotate(object.transform, glm::radians(static_cast<float>(x)));
	return object;
}

Graphic tkom::lib::unFill(Graphic object)
{
	object.fill = false;
	return object;
}

Color tkom::lib::colorRGB(int r, int g, int b)
{
	Color color;
	color.color = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
	return color;
}

Graphic tkom::lib::setColor(Graphic obj, Color c)
{
	obj.color = c;
	return obj;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int tkom::lib::show(Graphic image)
{
	if (glfwInit() != GL_TRUE)
		throw Error();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		GLFWwindow* window = initWindow(image.width, image.height);
		GLuint shaderProgram = compileShaders();
		image.start();

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents(); 
			glClearColor(image.color.color.x, image.color.color.y, image.color.color.z, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			image.render(shaderProgram, glm::mat3(1.0f));

			glfwSwapBuffers(window);
		}
		image.destroy();
	}
	catch (std::exception ex)
	{ std::cout << ex.what() << std::endl; }
	
	glfwTerminate();

	return 0;
}


GLuint compileShaders()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::string msg = std::string("Vertex shader compilation: ") + infoLog;
		throw std::exception(msg.c_str());
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::string msg = std::string("Fragment shader compilation: ") + infoLog;
		throw std::exception(msg.c_str());
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::string msg = std::string("Shader linking: ") + infoLog;
		throw std::exception(msg.c_str());
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLFWwindow* initWindow(GLuint w, GLuint h)
{
	GLFWwindow* window = glfwCreateWindow(w, h, "Press ESC to exit", nullptr, nullptr);
	if (window == nullptr)
		throw std::exception("GLFW window not created");
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::exception("GLEW Initialization failed");

	glViewport(0, 0, w, h);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	return window;
}