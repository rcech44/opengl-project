#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include others
#include <vector>
#include <map>
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
	std::vector<Light> lights;
	std::vector<DrawableObject> objects;
	std::vector<DrawableObject> lightObjects;
	std::vector<DrawableObject> skyboxes;
	std::map<int, Texture*> textures;
	std::map<int, Shader*> shaders;
	std::map<int, Model*> models;
	bool flashlight = true;
	int object_id = 0;

public:
	Scene() {};

	void addObject(DrawableObject* obj);
	void addSkybox(DrawableObject* obj);
	void addShader(Shader* sh, int id);
	void addModel(Model* m, int id);
	void addLight(Light* l);
	void addTexture(Texture* t, int id);
	void placeNewObject(glm::vec3 pos, int object_name);

	Camera* getCamera();
	std::vector<Light>* getLights();
	bool flashlightStatus();

	void update();
	void init();
	void toggleFlashlight();
};

