#include "Model.h"
#include <stdio.h>

// For loading objects from header file
Model::Model(int name, int model_type, const float* p, size_t size, int type, int number_of_objects, int coords_size, int color_size)
{
	this->model_name = name;
	this->model_type = model_type;
	this->type = type;
	this->points = p;
	this->points_size = size;
	this->number_of_objects = number_of_objects;
	this->coords_size = coords_size;
	this->color_size = color_size;
}

// For loading objects from .obj file
Model::Model(int name, int model_type, const char* path, int type, int coords_size, int color_size)
{
	this->model_name = name;
	this->model_type = model_type;
	this->type = type;
	this->path = path;
	this->coords_size = coords_size;
	this->color_size = color_size;
}

int Model::getName()
{
	return this->model_name;
}

void Model::set()
{
	printf("[OBJECTS] Creating VAO and VBO...\n");

	//vertex buffer object (VBO)
	GLuint temp_VBO = 0;
	glGenBuffers(1, &temp_VBO);				// generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, temp_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->points_size, this->points, GL_STATIC_DRAW);
	this->VBO = temp_VBO;

	//Vertex Array Object (VAO)
	GLuint temp_VAO = 0;
	glGenVertexArrays(1, &temp_VAO);		// generate the VAO
	glBindVertexArray(temp_VAO);			// bind the VAO
	this->VAO = temp_VAO;

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size) * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size) * sizeof(float), (void*)(coords_size * sizeof(float)));

	printf("[OBJECTS] Created VAO and VBO for model\n");
}

void Model::setSkyBox()
{
	printf("[OBJECTS] Creating VAO and VBO...\n");

	//vertex buffer object (VBO)
	GLuint temp_VBO = 0;
	glGenBuffers(1, &temp_VBO);				// generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, temp_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->points_size, this->points, GL_STATIC_DRAW);
	this->VBO = temp_VBO;

	//Vertex Array Object (VAO)
	GLuint temp_VAO = 0;
	glGenVertexArrays(1, &temp_VAO);		// generate the VAO
	glBindVertexArray(temp_VAO);			// bind the VAO
	this->VAO = temp_VAO;

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size) * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	printf("[OBJECTS] Created VAO and VBO for model\n");
}

void Model::setWithTexture()
{
	printf("[OBJECTS] Creating VAO and VBO...\n");

	//vertex buffer object (VBO)
	GLuint temp_VBO = 0;
	glGenBuffers(1, &temp_VBO);				// generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, temp_VBO);
	glBufferData(GL_ARRAY_BUFFER, this->points_size, this->points, GL_STATIC_DRAW);
	this->VBO = temp_VBO;

	//Vertex Array Object (VAO)
	GLuint temp_VAO = 0;
	glGenVertexArrays(1, &temp_VAO);		// generate the VAO
	glBindVertexArray(temp_VAO);			// bind the VAO
	this->VAO = temp_VAO;

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (void*)(coords_size * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (void*)((coords_size + color_size) * sizeof(float)));
	glEnableVertexAttribArray(2);

	printf("[OBJECTS] Created VAO and VBO for model\n");
}

void Model::setObject()
{
	Assimp::Importer importer;
	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_OptimizeMeshes              // slouèení malých plošek
		| aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodnì duplikuje
		| aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
		| aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy
	const aiScene* scene = importer.ReadFile(path, importOptions);
	if (scene) {
		aiMesh* mesh = scene->mMeshes[0];
		this->number_of_objects = mesh->mNumFaces * 3;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			for (unsigned int j = 0; j < 3; j++)
			{
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].z);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].z);
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].y);
			}
		}
	}
	//Vertex Array Object (VAO)
	GLuint VBO = 0;
	glGenBuffers(1, &VBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points_obj.size() * sizeof(float), &points_obj[0], GL_STATIC_DRAW);
	this->VBO = VBO;

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO); //generate the VAO
	glBindVertexArray(VAO); //bind the VAO
	this->VAO = VAO;

	//enable vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (GLvoid*)(sizeof(float) * color_size));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (GLvoid*)(sizeof(float) * (coords_size + color_size)));
	
}

void Model::setObjectWithNormals()
{
	Assimp::Importer importer;
	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_OptimizeMeshes              // slouèení malých plošek
		| aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodnì duplikuje
		| aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
		| aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy
	const aiScene* scene = importer.ReadFile(path, importOptions);
	if (scene) {
		aiMesh* mesh = scene->mMeshes[0];
		this->number_of_objects = mesh->mNumFaces * 3;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			for (unsigned int j = 0; j < 3; j++)
			{
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].z);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].z);
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].z);
			}
		}
	}

	//Vertex Array Object (VAO)
	GLuint VBO = 0;
	glGenBuffers(1, &VBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points_obj.size() * sizeof(float), &points_obj[0], GL_STATIC_DRAW);
	this->VBO = VBO;

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO); //generate the VAO
	glBindVertexArray(VAO); //bind the VAO
	this->VAO = VAO;

	//enable vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)(sizeof(float) * color_size));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)(sizeof(float) * (coords_size + color_size)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)(sizeof(float) * (coords_size + color_size + 2)));

}

void Model::render()
{
	switch (this->type)
	{
		case GL_POLYGON:
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_POLYGON, 0, number_of_objects);
			break;

		case GL_QUADS:
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_QUADS, 0, number_of_objects);
			break;

		case GL_TRIANGLES:
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, number_of_objects);
			break;
	}
}
