#include "ObjectMovement.h"
#include "DrawableObject.h"

ObjectMovement::ObjectMovement(glm::vec3 from, glm::vec3 to, double speed)
{
	this->from_x = from.x;
	this->from_y = from.y;
	this->from_z = from.z;
	this->to_x = to.x;
	this->to_y = to.y;
	this->to_z = to.z;
	this->speed = speed;
	this->type = Line;
}

ObjectMovement::ObjectMovement(glm::vec3 point, double radius, double speed)
{
	this->point = point;
	this->orbit_radius = radius;
	this->speed = speed;
	this->type = Orbit;
	this->angle = 0;
}

void ObjectMovement::move(DrawableObject* obj)
{
	switch (this->type)
	{
		case Line:
			obj->addTransformation(glm::vec3(
				(to_x - from_x) * speed,
				(to_y - from_y) * speed,
				(to_z - from_z) * speed
			), Translation);
			break;
		case Orbit:
			obj->setPosition(glm::vec3(
				glm::cos(glm::radians(this->angle)) * orbit_radius + point.x,
				point.y,
				glm::sin(glm::radians(this->angle)) * orbit_radius + point.z)
			);
			obj->addTransformation(glm::vec3(0, (glm::radians(-this->angle)), 0), Rotation);
			this->angle += speed;
			break;
	}
}