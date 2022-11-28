#pragma once
#include "Window.h"

//Include GLFW  
#include <GLFW/glfw3.h>  
#include <stdlib.h>  

Window::Window(int width, int height, const char* name)
{
	// Create new window
	this->width = width;
	this->height = height;
	this->window = glfwCreateWindow(width, height, name, NULL, NULL);

	// Check if window was successfully created
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
	// Specify that we are working with this window - context
	glfwMakeContextCurrent(this->window);
}

void Window::setInterval()
{
	// Swap interval - number of screen updates before we swap buffers
	glfwSwapInterval(1);
}

void Window::setViewport()
{
	// Setting viewport - location in window where we show graphic results
	int width, height;
	glfwGetFramebufferSize(this->window, &width, &height);
	glViewport(0, 0, width, height);
}

void Window::swapBuffers()
{
	// Swap frame buffers
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
	// Change size handler, will adjust viewport to new size of the window
	setViewport();
	this->width = width;
	this->height = height;
}

