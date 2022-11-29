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

//Include classes
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include "Transformation.h"

class Scene;
class ObjectMovement;

class DrawableObject
{
private:
	glm::vec3 color{ 1.0f, 1.0f, 1.0f };
	glm::mat4 last_position{ 1.f };
	std::vector<Transformation> transformations;
	int id = -1;
	const char* name = nullptr;

	// references
	Scene* scene = nullptr;
	Model* model = nullptr;
	Shader* shader = nullptr;
	ObjectMovement* movement = nullptr;

	// optional
	int texture = -1;
	int normal = -1;

public:
	DrawableObject(Model* m, Shader* s, Scene* scene, int id);
	void render();

	glm::mat4 transform();
	void addTransformation(glm::vec3 tf, int type);
	void setPosition(glm::vec3 pos);
	void setColor(glm::vec3 color);
	void assignTexture(int t);
	void assignTexture(int t, int n);
	void assignMovement(ObjectMovement* m);
	void move();
	int getID();
	const char* getName();
};

