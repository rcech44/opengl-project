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

class Scene;

class DrawableObject
{
private:
	glm::vec3 translation{ 0.f };
	glm::vec3 rotation{ 0.f };
	glm::vec3 size{ 1.f };
	glm::vec3 color{ 1.0f, 1.0f, 1.0f };

	// references
	Scene* scene = nullptr;
	Model* model = nullptr;
	Shader* shader = nullptr;

	// optional
	Light* light = nullptr;

	// other properties - unused
	float auto_rotate_multiplier = -1.f;

public:
	DrawableObject(Model* m, Shader* s, Scene* scene);
	void render();

	glm::mat4 transform();
	void rotate(glm::vec3 rot);
	void setPosition(glm::vec3 pos);
	void addRotation(glm::vec3 rot);
	void move(glm::vec3 difference);
	void scale(glm::vec3 scale);
	void setAutoRotateSettings(float multiplier);
	void setColor(glm::vec3 color);
	void assignLight(Light& l);
};

