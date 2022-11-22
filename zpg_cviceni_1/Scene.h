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
	std::vector<Light> lights;
	std::vector<DrawableObject> objects;
	std::vector<DrawableObject> lightObjects;
	std::vector<DrawableObject> skyboxes;
	std::vector<Texture> textures;
	std::vector<Shader*> shaders;
	std::vector<Model*> models;
	bool flashlight = true;
	int object_id = 0;

public:
	Scene() {};
	void addObject(DrawableObject* obj);
	void placeNewObject(glm::vec3 pos, int object_name);
	void addLightObject(DrawableObject* obj);
	void addSkybox(DrawableObject* obj);
	void addShader(Shader* sh);
	void addModel(Model* m);
	Model* getModel(int index);
	Shader* getShader(int index);
	void addLight(Light* l);
	void addTexture(Texture* t);
	Camera* getCamera();
	std::vector<Light>* getLights();
	void update();
	void init();
	void toggleFlashlight();
	bool flashlightStatus();

	float orbit = 1.f;
};

