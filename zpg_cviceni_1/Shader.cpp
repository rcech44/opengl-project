#pragma once

#include "Shader.h"
#include "Camera.h"
#include <stdio.h>

Shader::Shader(const char** fs, const char** vs, int type)
{
	// Replaced by shader loading from file

	/*this->fragment_shader_code = fs;
	this->vertex_shader_code = vs;
	this->shader_type = type;*/
}

Shader::Shader(const char* fs_loc, const char* vs_loc, int type)
{
	// https://stackoverflow.com/questions/18398167/how-to-copy-a-txt-file-to-a-char-array-in-c

	std::ifstream raw_fs(fs_loc);
	if (!raw_fs)
	{
		printf("[SHADER] Shader could not be loaded - file: %s\n", fs_loc);
		exit(-1);
	}
	std::ifstream raw_vs(vs_loc);
	if (!raw_vs)
	{
		printf("[SHADER] Shader could not be loaded - file: %s\n", vs_loc);
		exit(-1);
	}

	std::string dumped_fs((std::istreambuf_iterator<char>(raw_fs)), std::istreambuf_iterator<char>());
	this->fragment_shader_code = dumped_fs;

	std::string dumped_vs((std::istreambuf_iterator<char>(raw_vs)), std::istreambuf_iterator<char>());
	this->vertex_shader_code = dumped_vs;

	this->shader_type = type;
}

void Shader::compileShaders()
{
	glCompileShader(this->fragment_shader_id);
	glCompileShader(this->vertex_shader_id);
}

void Shader::createProgram()
{
	GLuint prog = glCreateProgram();
	glAttachShader(prog, fragment_shader_id);
	glAttachShader(prog, vertex_shader_id);

	glLinkProgram(prog);
	this->program = prog;

	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(prog, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "[SHADERS] Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	else printf("[SHADERS] Shaders passed compilation and linking check\n");
}

void Shader::createShaders()
{
	const char* fs = this->fragment_shader_code.c_str();
	const char* vs = this->vertex_shader_code.c_str();

	// fragment shader
	this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->fragment_shader_id, 1, &fs, NULL);

	// vertex shader
	this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->vertex_shader_id, 1, &vs, NULL);
}

void Shader::set()
{
	createShaders();
	compileShaders();
	createProgram();
}

void Shader::matrixInsert(glm::mat4 M, const char* variable)
{
	GLint idModelTransform = glGetUniformLocation(this->program, variable);
	if (idModelTransform == -1) printf("ERROR");
	glUniformMatrix4fv(idModelTransform, 1, GL_FALSE, &M[0][0]);
}

void Shader::vec3Insert(glm::vec3 &value, const char* variable)
{
	if (this->shader_type == StandardObject || variable == "objectColor")
	{
		GLint idModelTransform = glGetUniformLocation(this->program, variable);
		if (idModelTransform == -1) printf("ERROR");
		glUniform3fv(idModelTransform, 1, &value[0]);
	}
}

void Shader::useProgram()
{
	glUseProgram(this->program);
}

int Shader::getShaderType()
{
	return this->shader_type;
}

void Shader::notify(EventType eventType, void* object)
{
	if (eventType == EventType::CameraMoved)
	{
		Camera& camera = *((Camera*)object);
		matrixInsert(camera.getView(), ViewMatrix);
		matrixInsert(camera.getProjection(), ProjectionMatrix);
	}
}
