#include "Mouse.h"

void Mouse::calculateDirection()
{
	glm::vec3 dir;
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->direction = glm::normalize(dir);
}

void Mouse::apply()
{
	notifyObservers(EventType::MouseMoved, this);
}

void Mouse::calculatePosition(double x, double y)
{
	if (ever_moved == false)
	{
		x_last = x;
		y_last = y;
		ever_moved = true;
	}

	x_offset = x - x_last;
	y_offset = y - y_last;
	x_last = x;
	y_last = y;

	x_offset *= sensitivity;
	y_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw += (float)x_offset;
	pitch -= (float)y_offset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
}

glm::vec3 Mouse::getDirection()
{
	return direction;
}

double Mouse::getX()
{
	return this->x;
}

double Mouse::getY()
{
	return this->y;
}
