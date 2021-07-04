#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 1024, HEIGHT = 768;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

// shaders
static const char* vertexShader =
	"#version 330\n"
	"layout(location = 0) in vec3 pos;\n"
	"uniform mat4 model;\n"
	"void main() {\n"
		"gl_Position = model * vec4(0.4f * pos.x, 0.4f * pos.y, pos.z, 1.0);\n"
	"}";

static const char* fragmentShader =
	"#version 330\n"
	"out vec4 color;\n"
	"void main() {\n"
		"color = vec4(1.0f, 0.0f, 1.0f, 0.8f);\n"
	"}";

void createTriangle() {
	float vertices[9] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GL_FLOAT), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// unbind VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void addShader(GLint program, const char* shaderCode, GLenum type) {
	GLuint theShader = glCreateShader(type);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	glAttachShader(program, theShader);
}

void compileShaders() {
	shader = glCreateProgram();		// global var
	if (!shader) {
		std::cerr << "Unable to create program\n";
		exit(EXIT_FAILURE);
	}

	addShader(shader, vertexShader, GL_VERTEX_SHADER);
	addShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	// create GPU executables
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		std::cout << "Error linking: " << elog << std::endl;
		exit(EXIT_FAILURE);
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		std::cout << "Error validating: " << elog << std::endl;
		exit(EXIT_FAILURE);
	}

	uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
	if (!glfwInit()) {
		std::cerr << "glfw not initialised\n";
		return EXIT_FAILURE;
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "my app", NULL, NULL);
	if (!window) {
		std::cerr << "Unable to create window\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK) {
		std::cerr << "glew not initialised\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	createTriangle();
	compileShaders();

	// game loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		if (!direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) 
		{
			direction = !direction;
		}

		curAngle += 0.01f;

		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw!
		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// clear shader
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}