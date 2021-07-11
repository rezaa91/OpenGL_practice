#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>

class Shader
{
public:
	Shader();
	void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
	GLuint getProjectionLocation();
	GLuint getModelLocation();
	void useShader();
	void clearShader();
	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel;
	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLint program, const char* shaderCode, GLenum type);
};

