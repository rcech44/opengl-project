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

#include "Observer.h"

class Mouse : public Observable
{
private:
	float yaw = -90.0f;				// angle X
	float pitch = 0.0f;				// angle Y
	float sensitivity = 0.4f;
	double x = 0;
	double y = 0;
	double x_last = 0;
	double y_last = 0;
	double x_offset = 0;
	double y_offset = 0;
	glm::vec3 direction{0.f};

	bool ever_moved = false;

public:
	Mouse() {};
	void calculateDirection();
	void apply();
	void calculatePosition(double x, double y);
	glm::vec3 getDirection();
	double getX();
	double getY();
};

