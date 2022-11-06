// *
// Extension of Application class from Application.h
// *

#pragma once

#include "Application.h"

void Application::error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		printf("[INIT] Exiting...\n");
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
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
			/*if (action == GLFW_PRESS)
			{
				this->mouse_before_x = mouse.getX();
				this->mouse_before_y = mouse.getY();
				this->mouseChangeState();
				if (this->mouse_enabled) glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else glfwSetInputMode(this->window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(window, mouse_before_x, mouse_before_y);
			}*/
			break;

		default:
			//printf("Pressed key: [%d,%d,%d,%d] \n", key, scancode, action, mods);
			break;
	}
}

void Application::window_focus_callback(GLFWwindow* window, int focused)
{
	printf("Changed window focus: %s \n", focused ? "focused" : "out of focus");
}

; void Application::window_iconify_callback(GLFWwindow* window, int iconified)
{
	printf("Window state changed: %s\n", iconified ? "minimized" : "not minimized");
}

void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
	printf("Window resized: %d, %d \n", width, height);
	this->window.changeSize(height, width);
	this->scene.getCamera()->setProjection(width, height);
}

void Application::cursor_callback(GLFWwindow* window, double x, double y)
{
	mouse.calculatePosition(x, y);
	mouse.calculateDirection();
	mouse.apply();
	//printf("Cursor location changed: %f, %f \n", x, y);
}

void Application::button_callback(GLFWwindow* window, int button, int action, int mode)
{
	//if (button == GLFW_MOUSE_BUTTON_LEFT) {
	//	if (GLFW_PRESS == action)
	//		this->mouse_enabled = true;
	//	else if (GLFW_RELEASE == action)
	//		this->mouse_enabled = false;
	//}
	//if (action == GLFW) printf("Pressed mouse button: [%d,%d,%d]\n", button, action, mode);
}
