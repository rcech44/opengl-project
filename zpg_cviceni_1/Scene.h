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
#include "Light.h"

class Scene
{
private:
	Camera camera;

public:
	std::vector<DrawableObject> objects;
	std::vector<Light> lights;
	Scene() {};
	void addObject(DrawableObject* obj);
	void addLight(Light* l);
	Camera* getCamera();
	std::vector<Light>* getLights();
	void update();
	void init();
};

