#include "Model.h"
#include <stdio.h>

Model::Model(const float* p, size_t size, int type, int number_of_objects, int coords_size, int color_size)
{
	this->type = type;
	this->points = p;
	this->points_size = size;
	this->number_of_objects = number_of_objects;
	this->coords_size = coords_size;
	this->color_size = color_size;
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
