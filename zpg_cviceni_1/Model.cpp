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
	// Create VBO and VAO
	createVAOVBO();

	// Enable vertex attributes and specify their content
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size) * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size) * sizeof(float), (void*)(coords_size * sizeof(float)));
}

void Model::setSkyBox()
{
	// Create VBO and VAO
	createVAOVBO();

	// Enable vertex attributes and specify their content
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size) * sizeof(float), (void*)0);
}

void Model::setWithTexture()
{
	// Create VBO and VAO
	createVAOVBO();

	// Enable vertex attributes and specify their content
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (void*)(coords_size * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (void*)((coords_size + color_size) * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Model::setObjectTest()
{
	int indicesCount = 0;

	struct Vertex
	{
		float Position[3];
		float Normal[3];
		float Texture[2];
		float Tangent[3];
	};

	Assimp::Importer importer;
	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_OptimizeMeshes              // slouèení malých plošek
		| aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodnì duplikuje
		| aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
		| aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy

	//aiProcess_GenNormals/ai_Process_GenSmoothNormals - vypocet normal s jemnych prechodem v pripade, ze objekt neobsahuje normaly

	const aiScene* scene = importer.ReadFile(path, importOptions);

	if (scene) { //pokud bylo nacteni uspesne
		printf("scene->mNumMeshes = %d\n", scene->mNumMeshes);
		printf("scene->mNumMaterials = %d\n", scene->mNumMaterials);

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)                       //Materials
		{
			const aiMaterial* mat = scene->mMaterials[i];

			aiString name;
			mat->Get(AI_MATKEY_NAME, name);
			printf("Material [%d] name %s\n", i, name.C_Str());

			aiColor4D d;

			glm::vec4 diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
			if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &d))
				diffuse = glm::vec4(d.r, d.g, d.b, d.a);

		}

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)                      //Objects
		{
			aiMesh* mesh = scene->mMeshes[i];

			Vertex* pVertices = new Vertex[mesh->mNumVertices];
			std::memset(pVertices, 0, sizeof(Vertex) * mesh->mNumVertices);

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				if (mesh->HasPositions())
				{
					pVertices[i].Position[0] = mesh->mVertices[i].x;
					pVertices[i].Position[1] = mesh->mVertices[i].y;
					pVertices[i].Position[2] = mesh->mVertices[i].z;
				}
				if (mesh->HasNormals())
				{
					pVertices[i].Normal[0] = mesh->mNormals[i].x;
					pVertices[i].Normal[1] = mesh->mNormals[i].y;
					pVertices[i].Normal[2] = mesh->mNormals[i].z;
				}
				if (mesh->HasTextureCoords(0))
				{
					pVertices[i].Texture[0] = mesh->mTextureCoords[0][i].x;
					pVertices[i].Texture[1] = mesh->mTextureCoords[0][i].y;
				}
				if (mesh->HasTangentsAndBitangents())
				{
					pVertices[i].Tangent[0] = mesh->mTangents[i].x;
					pVertices[i].Tangent[1] = mesh->mTangents[i].y;
					pVertices[i].Tangent[2] = mesh->mTangents[i].z;
				}

			}

			unsigned int* pIndices = nullptr;

			if (mesh->HasFaces())
			{

				pIndices = new unsigned int[mesh->mNumFaces * 3];
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					pIndices[i * 3] = mesh->mFaces[i].mIndices[0];
					pIndices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
					pIndices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
				}
			}

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &IBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mNumVertices, pVertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(0));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8 * sizeof(GLfloat)));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, pIndices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(VAO);

			GLuint err = glGetError();
			if (err != GL_NO_ERROR)
			{
				printf("ERROR LOAD\n");
				return;
			} number_of_objects = mesh->mNumFaces * 3;

			delete[] pVertices;
			delete[] pIndices;
		}
	}
	else {
		printf("Error during parsing mesh from %s : %s \n", path, importer.GetErrorString());
	}
	glBindVertexArray(0);
}

void Model::setObject()
{
	// Create new importer and define settings
	Assimp::Importer importer;
	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_OptimizeMeshes              // slouèení malých plošek
		| aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodnì duplikuje
		| aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
		| aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy

	// Read .obj file
	const aiScene* scene = importer.ReadFile(path, importOptions);

	// If read correctly, process it
	if (scene) {
		// Take only first mesh
		aiMesh* mesh = scene->mMeshes[0];

		// Save number of objects - number of triangles in mesh * 3
		this->number_of_objects = mesh->mNumFaces * 3;

		// Go through all triangles
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			// Go through all points of triangle
			for (unsigned int j = 0; j < 3; j++)
			{
				// Coordinates of point
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].z);

				// Get normals as normalized vectors
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].z);

				// Get texture coordinates (UV)
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].y);
			}
		}
	}

	// Save information and points
	this->points_size = points_obj.size() * sizeof(float);
	this->points = &points_obj[0];

	// Create VBO and VAO
	createVAOVBO();

	// Enable vertex attributes and specify their content
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (GLvoid*)(sizeof(float) * color_size));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2) * sizeof(float), (GLvoid*)(sizeof(float) * (coords_size + color_size)));
	
}

void Model::setObjectWithNormals()
{
	// Create new importer and define settings
	Assimp::Importer importer;
	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_OptimizeMeshes              // slouèení malých plošek
		| aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodnì duplikuje
		| aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
		| aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy

	// Read .obj file
	const aiScene* scene = importer.ReadFile(path, importOptions);

	// If read correctly, process it
	if (scene) {
		// Take only first mesh
		aiMesh* mesh = scene->mMeshes[0];

		// Save number of objects - number of triangles in mesh * 3
		this->number_of_objects = mesh->mNumFaces * 3;

		// Go through all triangles
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
		{
			// Go through all points of triangle
			for (unsigned int j = 0; j < 3; j++)
			{
				// Coordinates of point
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].z);

				// Get normals as normalized vectors
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].z);

				// Get texture coordinates (UV)
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].y);

				// Get tangents
				points_obj.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].x);
				points_obj.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].y);
				points_obj.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].z);
			}
		}
	}

	// Save information and points
	this->points_size = points_obj.size() * sizeof(float);
	this->points = &points_obj[0];

	// Create VBO and VAO
	createVAOVBO();

	// Enable vertex attributes and specify their content
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, coords_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, color_size, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)(sizeof(float) * color_size));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)(sizeof(float) * (coords_size + color_size)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (coords_size + color_size + 2 + 3) * sizeof(float), (GLvoid*)(sizeof(float) * (coords_size + color_size + 2)));

}

void Model::createVAOVBO()
{
	// Vertex Buffer Object (VAO) - GPU memory that holds information about vertices
	GLuint VBO = 0;

	// Generate new VBO
	glGenBuffers(1, &VBO);

	// Bind it as current buffer - so we can work with it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy data to buffer (GL_STATIC_DRAW = the data is set only once and used many times)
	glBufferData(GL_ARRAY_BUFFER, this->points_size, this->points, GL_STATIC_DRAW);

	// Save ID of newly created VBO
	this->VBO = VBO;


	// Vertex Array Object (VAO) - "instructions" how to use data in VBO
	GLuint VAO = 0;

	// Generate new VAO
	glGenVertexArrays(1, &VAO);

	// Bind it as current VAO - so we can work with it
	glBindVertexArray(VAO);

	// Save its ID
	this->VAO = VAO;

	printf("[OBJECTS] Created VAO and VBO for model\n");
}

void Model::render()
{
	// Render object
	switch (this->type)
	{
		case GL_TRIANGLES:
			// Specify the VAO of object
			glBindVertexArray(this->VAO);
			if (IBO != -1) glDrawElements(GL_TRIANGLES, number_of_objects, GL_UNSIGNED_INT, NULL);
			else glDrawArrays(GL_TRIANGLES, 0, number_of_objects);
			break;
	}
}
