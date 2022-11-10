#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include others
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>

//Include classes
#include "Observer.h"

#define ViewMatrix "viewMatrix"
#define ProjectionMatrix "projectionMatrix"
#define TransformMatrix "modelMatrix"
#define LightPosition "lightPos"
#define LightColor "lightColor"
#define ObjectColor "objectColor"

#define StandardObject 1			// calculates light and colors
#define LightSource 2				// solid color block without light calculations
#define ConstantObject 3			// solid color block without light calculations
#define StandardObjectTextured 4	// solid color block without light calculations
#define SkyBox 5					// solid color block without light calculations

class Shader : public Observer
{
private:
	std::string fragment_shader_code = "";
	std::string vertex_shader_code = "";
	GLuint fragment_shader_id = -1;
	GLuint vertex_shader_id = -1;
	GLuint program = -1;
	int shader_type = 1;
	glm::mat4 last_camera_view;
	glm::mat4 last_camera_projection;

public:
	Shader(const char** fs, const char** vs, int type);
	Shader(const char* fs_loc, const char* vs_loc, int type);

	void set();
	void matrixInsert(glm::mat4 M, const char* variable);
	void vec3Insert(glm::vec3& value, const char* variable);
	void intInsert(int value, const char* variable);
	void floatInsert(float value, const char* variable);
	void createShaders();
	void compileShaders();
	void createProgram();
	void useProgram();
	int getShaderType();
	void applyCamera();

	// Observer
	void notify(EventType eventType, void* object) override;
};