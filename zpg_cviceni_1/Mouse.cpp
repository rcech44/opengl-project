#include "Mouse.h"

void Mouse::calculateDirection()
{
	glm::vec3 dir;

	// https://learnopengl.com/Getting-started/Camera
	// X axis = cosinus of yaw (adjacent), also influenced by pitch
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));

	// Sinus of pitch (height of Y)
	dir.y = sin(glm::radians(pitch));

	// Z axis = sinus of yaw (opposite), also influenced by pitch
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->direction = dir;
}

void Mouse::apply()
{
	notifyObservers(EventType::MouseMoved, this);
}

void Mouse::calculatePosition(double x, double y)
{
	// Initial config if mouse has not yet moved
	if (ever_moved == false)
	{
		x_last = x;
		y_last = y;
		ever_moved = true;
	}

	// Calculate offsets and set last positions
	x_offset = x - x_last;
	y_offset = y - y_last;
	x_last = x;
	y_last = y;

	// Modify offsets by multiplier
	x_offset *= sensitivity;
	y_offset *= sensitivity;
	y_offset *= sensitivity;

	// Moving to sides
	yaw += (float)x_offset;

	// Moving up or down
	pitch -= (float)y_offset;

	// Do not allow roll over top or bottom
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

void Mouse::switchEverMoved()
{
	this->ever_moved = false;
}
