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

enum ModelType
{
	HeaderType = 0,
	ObjectType = 1
};

enum ModelName
{
	Monkey = 0,
	Sphere = 1,
	Plain = 2,
	Tree = 3,
	Bush = 4,
	GiftBox = 5,
	SkyCube = 6,
	Building = 7,
	Zombie = 8,
	Tree2 = 9,
	Plain2 = 10,
	Box = 11
};

class Model
{
private:
	int model_type = -1;
	int model_name = -1;
	int type = -1;
	GLuint VAO = -1;
	GLuint VBO = -1;
	const float* points = nullptr;
	const char* path = nullptr;
	std::vector<float> points_obj;
	size_t points_size = -1;
	int number_of_objects = 0;
	int coords_size = 0;
	int color_size = 0;

public:
	Model(int name, int model_type, const float* p, size_t size, int type, int number_of_objects, int coords_size, int color_size);
	Model(int name, int model_type, const char* path, int type, int coords_size, int color_size);
	void set();
	void setSkyBox();
	void setWithTexture();
	void setObject();
	void setObjectWithNormals();
	int getName();
	void createVAOVBO();
	void render();
};

