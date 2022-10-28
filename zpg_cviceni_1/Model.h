#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  
#include <glm/vec3.hpp> // glm::vec3

class Model
{
private:
	int type = -1;
	GLuint VAO = -1;
	GLuint VBO = -1;
	const float* points = nullptr;
	size_t points_size = -1;
	int number_of_objects = 0;
	int coords_size = 0;
	int color_size = 0;

public:
	Model() {};
	Model(const float* p, size_t size, int type, int number_of_objects, int coords_size, int color_size);
	void set();
	void render();
};

