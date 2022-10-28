#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include others
#include <vector>
#include <cstdlib>
#include <ctime>

//Include classes
#include "DrawableObject.h"
#include "Camera.h"

class Scene
{
private:
	Camera camera;

public:
	Scene() {};
	std::vector<DrawableObject> objects;
	void addObject(DrawableObject* obj);
	Camera* getCamera();
	void update();
	void init();
};

