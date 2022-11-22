#include "Camera.h"
#include "Mouse.h"

Camera::Camera()
{
	camera = glm::lookAt(glm::vec3(0.0f, 0.0, 4.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::setPosition(glm::vec3 position)
{
	eye = position;
	apply();
}

void Camera::moveForward(Direction dir, double time)
{
	if(dir == Direction::forward) this->eye += (float)time * this->target * cameraMoveMultiplier;
	if(dir == Direction::backward) this->eye -= (float)time * this->target * cameraMoveMultiplier;
	printf("%f %f\n", eye.x, eye.z);
	//eye.z -= (float)((int)dir * time * cameraMoveMultiplier);
	changed = true;
	apply();
}

void Camera::moveSideways(Direction dir, double time)
{
	if (dir == Direction::left) this->eye -= glm::normalize(glm::cross(this->target, this->up)) * (float)time * cameraMoveMultiplier;
	if (dir == Direction::right) this->eye += glm::normalize(glm::cross(this->target, this->up)) * (float)time * cameraMoveMultiplier;
	//eye.x += (float)((int)dir * time * cameraMoveMultiplier);
	changed = true;
	apply();
}

void Camera::moveHeight(Direction dir, double time)
{
	if (dir == Direction::down) this->eye.y -= (float)time * cameraYMoveMultiplier;
	if (dir == Direction::up) this->eye.y += (float)time * cameraYMoveMultiplier;
	//eye.z -= (float)((int)dir * time * cameraMoveMultiplier);
	changed = true;
	apply();
}

void Camera::apply()
{
	this->camera = glm::lookAt(eye, eye + target, up);
	notifyObservers(EventType::CameraMoved, this);
}

glm::mat4 Camera::getView()
{
	return camera;
}

glm::mat4 Camera::getProjection()
{
	return projectMatrix;
}

glm::vec3 Camera::getPosition()
{
	return this->eye;
}

glm::vec3 Camera::getTarget()
{
	return this->target;
}

void Camera::setProjection(int width, int height)
{
	this->projectMatrix = glm::perspective(glm::radians(80.0f), (float)width / (float)height, 0.1f, 200.0f);
}

void Camera::notify(EventType eventType, void* object)
{
	if (eventType == EventType::MouseMoved) 
	{
		glm::vec3 dir = ((Mouse*)object)->getDirection();
		this->target = dir;
		this->changed = true;
		this->apply();
	}
}