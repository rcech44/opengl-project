#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  
#include <glm/vec3.hpp> // glm::vec3

//Include SOIL
#include <SOIL.h>

//Include ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Include others
#include <vector>
//#include "DrawableObject.h"

class DrawableObject;

enum MovementType
{
	Line = 0,
	Orbit = 1,
	OrbitVertical = 2
};

class ObjectMovement
{
private:
	int type = 0;
	
	// Line
	double from_x;
	double from_y;
	double from_z;
	double to_x;
	double to_y;
	double to_z;
	bool inverted;

	// Orbit
	double speed;
	double orbit_radius;
	double angle;
	glm::vec3 point;

public:
	ObjectMovement() {};
	ObjectMovement(glm::vec3 from, glm::vec3 to, double speed);
	ObjectMovement(glm::vec3 point, double radius, double speed, int type);
	void move(DrawableObject* obj);
};