#include "Transformation.h"

Transformation::Transformation(glm::vec3 vals, int type)
{
	this->values = vals;
	this->type = type;
}

Transformation::Transformation(glm::vec3 vals, float angle, int type)
{
	this->values = vals;
	this->type = type;
	this->angle = angle;
}

int Transformation::getType()
{
	return this->type;
}

glm::vec3 Transformation::getValues()
{
	return this->values;
}

float Transformation::getAngle()
{
	return angle;
}
