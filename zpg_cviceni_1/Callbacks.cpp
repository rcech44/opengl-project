// *
// Extension of Application class from Application.h
// *

#pragma once

#include "Application.h"

void Application::error_callback(int error, const char* description)
{
	// Print error in STDERR when error occures
	fputs(description, stderr);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// If ESCAPE key is pressed, handle exit application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		printf("[INIT] Exiting...\n");
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	// If key is hold, process movement and flashlight
	if (action != GLFW_PRESS)
	switch (key)
	{
		case GLFW_KEY_W:
			scene.getCamera()->moveForward(Direction::forward, last_frame_time);
			//printf("Pressed W\n");
			break;

		case GLFW_KEY_A:
			scene.getCamera()->moveSideways(Direction::left, last_frame_time);
			//printf("Pressed A\n");
			break;

		case GLFW_KEY_S:
			scene.getCamera()->moveForward(Direction::backward, last_frame_time);
			//printf("Pressed W\n");
			break;

		case GLFW_KEY_D:
			scene.getCamera()->moveSideways(Direction::right, last_frame_time);
			//printf("Pressed D\n");
			break;

		case GLFW_KEY_SPACE:
			scene.getCamera()->moveHeight(Direction::up, last_frame_time);
			//printf("Pressed D\n");
			break;

		case GLFW_KEY_LEFT_CONTROL:
			scene.getCamera()->moveHeight(Direction::down, last_frame_time);
			//printf("Pressed D\n");
			break;

		case GLFW_KEY_E:
			scene.toggleFlashlight();
			break;

		case GLFW_KEY_Q:
			if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			{
				mouse.switchEverMoved();
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;

		default:
			//printf("Pressed key: [%d,%d,%d,%d] \n", key, scancode, action, mods);
			break;
	}
}

// Callback when window focus changes (clicked outside of window)
void Application::window_focus_callback(GLFWwindow* window, int focused)
{
	printf("Changed window focus: %s \n", focused ? "focused" : "out of focus");
}

// Callback when window is minimized
; void Application::window_iconify_callback(GLFWwindow* window, int iconified)
{
	printf("Window state changed: %s\n", iconified ? "minimized" : "not minimized");
}

// Callback when window size changed
void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
	printf("Window resized: %d, %d \n", width, height);
	this->window.changeSize(height, width);
	this->scene.getCamera()->setProjection(width, height);
}

// Callback when mouse moved
void Application::cursor_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		mouse.calculatePosition(x, y);
		mouse.calculateDirection();
		mouse.apply();
	}
}

// Callback when mouse button was pressed
void Application::button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if ((GLFW_PRESS == action) && (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL))
	{
		// Get frame buffer size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Init needed variables
		GLfloat depth;
		GLuint index;

		// Calculate center of screen
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		y = height - y;

		// Read pixels from frame buffer
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
		
		// Point representing center of screen with depth
		glm::vec3 screenX = glm::vec3(x, y, depth);

		// Camera view - what camera sees
		glm::mat4 view = this->scene.getCamera()->getView();

		// Projection matrix
		glm::mat4 projection = this->scene.getCamera()->getProjection();

		// View port - location in window where scene is rendered
		glm::vec4 viewPort = glm::vec4(0, 0, width, height);

		// Calculating world coordinates from screen coordinates
		glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);

		// On mouse click place new objects
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			this->scene.placeNewObject(pos, Tree2, StandardObjectTextured);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			this->scene.placeNewObject(pos, Zombie, StandardObjectTextured);
		if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			this->scene.placeNewObject(pos, Sphere, ConstantObject);
	}
}
