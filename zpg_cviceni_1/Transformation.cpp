#include "Transformation.h"

Transformation::Transformation(glm::vec3 vals, int type)
{
	this->values = vals;
	this->type = type;
}

int Transformation::getType()
{
	return this->type;
}

glm::vec3 Transformation::getValues()
{
	return this->values;
}