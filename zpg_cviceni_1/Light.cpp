#include "Light.h"

Light::Light(int type)
{
	this->type = type;
}

void Light::setColor(glm::vec3 color)
{
	this->color = color;
}

void Light::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Light::setDirection(glm::vec3 dir)
{
	this->direction = dir;
}

void Light::setCutoff(float f)
{
	this->cutoff = f;
}

void Light::setOuterCutoff(float f)
{
	this->outer_cutoff = f;
}

void Light::setStrength(float f)
{
	this->strength = f;
}
