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
	this->total_steps = 1 / speed;
}

ObjectMovement::ObjectMovement(glm::vec3 point, double radius, double speed, int type)
{
	this->point = point;
	this->orbit_radius = radius;
	this->speed = speed;
	this->type = type;
	this->angle = 0;
	this->total_steps = 1 / speed;
}

void ObjectMovement::move(DrawableObject* obj)
{
	switch (this->type)
	{
		// Move on line
		case Line:
			if (!inverted)
			{
				obj->addTransformation(glm::vec3(
					(to_x - from_x) * speed,
					(to_y - from_y) * speed,
					(to_z - from_z) * speed
				), Translation);
			}
			else
			{
				obj->addTransformation(glm::vec3(
					-(to_x - from_x) * speed,
					-(to_y - from_y) * speed,
					-(to_z - from_z) * speed
				), Translation);
			}
			current_steps++;
			if (current_steps == total_steps)
			{
				current_steps = 0;
				inverted = !inverted;
			}
			break;

		// Orbit around object
		case Orbit:
			obj->setPosition(glm::vec3(
				glm::cos(glm::radians(this->angle)) * orbit_radius + point.x,	// move on X by cos of angle
				point.y,
				glm::sin(glm::radians(this->angle)) * orbit_radius + point.z)	// move on Z by sin of angle
			);
			obj->addTransformation(glm::vec3(0, (glm::radians(-this->angle)), 0), Rotation);	// rotate object around Y by angle
			this->angle += speed;
			break;

		// Orbit around object vertically
		case OrbitVertical:
			obj->setPosition(glm::vec3(
				glm::cos(glm::radians(this->angle)) * orbit_radius + point.x,	// move on X by cos of angle
				glm::sin(glm::radians(this->angle)) * orbit_radius + point.y,	// move on Y by sin of angle
				point.z
			));
			obj->addTransformation(glm::vec3(0, 0, (glm::radians(this->angle))), Rotation); // rotate object around Z by angle
			this->angle += speed;
			break;
	}
}