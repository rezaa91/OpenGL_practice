#include "Window.h"
#include <iostream>

Window::Window()
	:width(800), height(600)
{
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int Window::initialise()
{
	if (!glfwInit()) {
		std::cerr << "glfw not initialised\n";
		return EXIT_FAILURE;
	}

	mainWindow = glfwCreateWindow(width, height, "my app", NULL, NULL);
	if (!mainWindow) {
		std::cerr << "Unable to create window\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "glew not initialised\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
