#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

class Window
{
private:
	int width;
	int height;
	GLFWwindow* window;

public:
	Window() {};
	~Window() {};
	Window(int width, int height, const char* name);
	GLFWwindow* getWindow();
	void setContext();
	void setInterval();
	void setViewport();
	void swapBuffers();
	void deleteWindow();
	int shouldClose();
	void changeSize(int height, int width);
};

