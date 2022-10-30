#pragma once

#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, Shader* s)
{
	this->model = m;
	this->shader = s;
}

void DrawableObject::render(Camera* camera)
{
	glm::vec3 lightPos(0.f, 1.f, 0.f);
	glm::vec3 lightColor(0.8f, 0.8f, 0.8f);
	glm::vec3 viewPos = camera->getPosition();

	this->shader->useProgram();

	if (this->shader->getShaderType() == LightSource)
	{
		setPosition(glm::vec3(0.f, 50.f, 0.f));
		this->shader->vec3Insert(viewPos, "viewPos");
		this->shader->vec3Insert(this->color, "objectColor");
	}

	if (this->shader->getShaderType() == StandardObject)
	{
		//addRotation(glm::vec3(0.01,0,0));
		this->shader->vec3Insert(lightPos, "lightPos");
		this->shader->vec3Insert(lightColor, "lightColor");
		this->shader->vec3Insert(viewPos, "viewPos");
		this->shader->vec3Insert(this->color, "objectColor");
	}

	if (this->shader->getShaderType() == ConstantObject)
	{
		this->shader->vec3Insert(viewPos, "viewPos");
		this->shader->vec3Insert(this->color, "objectColor");
	}

	this->shader->matrixInsert(transform(), TransformMatrix);
	this->model->render();

	// unused
	//camera->apply();
	//glm::mat4 view = camera->getView();
}

glm::mat4 DrawableObject::transform()
{
	glm::mat4 t = glm::translate(glm::mat4{ 1.f }, translation);
	t = glm::scale(t, size);
	t = glm::rotate(t, rotation.x, { 1.f, 0.f, 0.f });
	t = glm::rotate(t, rotation.y, { 0.f, 1.f, 0.f });
	t = glm::rotate(t, rotation.z, { 0.f, 0.f, 1.f });
	return t;
}

void DrawableObject::rotate(glm::vec3 rot)
{
	this->rotation = rot;
}

void DrawableObject::setPosition(glm::vec3 pos)
{
	this->translation = pos;
}

void DrawableObject::addRotation(glm::vec3 rot)
{
	/*if (auto_rotate_multiplier > 0.f) this->rotation += rot * auto_rotate_multiplier;
	else if (auto_rotate_multiplier == 0.f) */
	this->rotation += rot;
}

void DrawableObject::move(glm::vec3 difference)
{
	this->translation += difference;
}

void DrawableObject::scale(glm::vec3 scale)
{
	this->size = scale;
}

void DrawableObject::setAutoRotateSettings(float multiplier)
{
	this->auto_rotate_multiplier = multiplier;
}

void DrawableObject::setColor(glm::vec3 color)
{
	this->color = color;
}