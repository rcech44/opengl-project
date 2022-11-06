#pragma once

//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include other headers
#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "DrawableObject.h"
#include "Scene.h"
#include "Observer.h"
#include "Mouse.h"
#include "Constants.h"
#include <vector>

class Application : public Observable
{
private:
	Application() {};
	static Application* instance;
	static Window window;
	Scene scene;
	Mouse mouse;
	double last_frame_time = 0;
	long frame_count = 0;
	double now_time = 0;

	// callbacks
	void setCallbacks();
	void error_callback(int error, const char* description);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void window_focus_callback(GLFWwindow* window, int focused);
	void window_iconify_callback(GLFWwindow* window, int iconified);
	void window_size_callback(GLFWwindow* window, int width, int height);
	void cursor_callback(GLFWwindow* window, double x, double y);
	void button_callback(GLFWwindow* window, int button, int action, int mode);

public:
	~Application() {};
	static Application* getInstance();
	void init();
	void initGLFW();
	void initGLEW();
	void setGLSettings();
	void loop();
	void printInfo();
	void debugFPS();
	void update();
};