#pragma once
#include "Window.h"

//Include GLFW  
#include <GLFW/glfw3.h>  
#include <stdlib.h>  

Window::Window(int width, int height, const char* name)
{
	this->width = width;
	this->height = height;
	this->window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (!this->window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	setContext();
	setInterval();
	setViewport();
}

GLFWwindow* Window::getWindow()
{
	return this->window;
}

void Window::setContext()
{
	glfwMakeContextCurrent(this->window);
}

void Window::setInterval()
{
	glfwSwapInterval(1);
}

void Window::setViewport()
{
	int width, height;
	glfwGetFramebufferSize(this->window, &width, &height);
	glViewport(0, 0, width, height);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(this->window);
}

void Window::deleteWindow()
{
	glfwDestroyWindow(this->window);
}

int Window::shouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Window::changeSize(int height, int width)
{
	setViewport();
	this->width = width;
	this->height = height;
}

