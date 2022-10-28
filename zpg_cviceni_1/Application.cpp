#pragma once

#include "Application.h"
#include <functional>

Window Application::window;
Application* Application::instance;

Application* Application::getInstance()
{
	if (!instance) instance = new Application();
	return instance;
}

void Application::init()
{
	// GLFW initialization
	initGLFW();

	// Window creation and cursor behavior
	window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Unreal Engine 6");
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Start GLEW extension handler and other stuff related to GL
	initGLEW();
	setGLSettings();

	// Create scene and init scene
	this->scene = Scene();
	this->scene.init();

	// Set callbacks
	this->setCallbacks();

	// Register needed observers
	mouse.registerObserver(*scene.getCamera());

	// Print debug info
	printInfo();
}

void Application::initGLFW()
{
	if (!glfwInit())
	{
		fprintf(stderr, "[INIT] Could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}
	else printf("[INIT] GLFW initialized successfully\n");
	glfwWindowHint(GLFW_SAMPLES, 4);
}

void Application::initGLEW()
{
	glewExperimental = GL_TRUE;
	glewInit();
}

void Application::setGLSettings()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
}

void Application::loop()
{
	while (!window.shouldClose())
	{
		now_time = glfwGetTime();
		update();
	}
	window.deleteWindow();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Application::printInfo()
{
	// get version info
	printf("[INIT] OpenGL Version: \t\t%s\n", glGetString(GL_VERSION));
	printf("[INIT] Using GLEW \t\t%s\n", glewGetString(GLEW_VERSION));
	printf("[INIT] Vendor \t\t\t%s\n", glGetString(GL_VENDOR));
	printf("[INIT] Renderer \t\t%s\n", glGetString(GL_RENDERER));
	printf("[INIT] GLSL \t\t\t%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("[INIT] Using GLFW \t\t%i.%i.%i\n", major, minor, revision);
}

void Application::debugFPS()
{
	last_frame_time = glfwGetTime() - now_time;
	if (frame_count % 200 == 1) printf("[FPS-DEBUG] FPS: %.0f\n", round((float)(1 / last_frame_time)));
	frame_count++;
}

void Application::update()
{
	scene.update();
	glfwPollEvents();
	window.swapBuffers();
	debugFPS();
}

void Application::mouseChangeState()
{
	this->mouse_enabled = !mouse_enabled;
}

void Application::setCallbacks()
{
	auto key = [](GLFWwindow* win, int key, int scancode, int action, int mods) {
		getInstance()->key_callback(win, key, scancode, action, mods);
	};
	glfwSetKeyCallback(window.getWindow(), key);

	auto cursor = [](GLFWwindow* win, double x, double y) {
		getInstance()->cursor_callback(win, x, y);
	};
	glfwSetCursorPosCallback(window.getWindow(), cursor);

	auto cursor_button = [](GLFWwindow* win, int button, int action, int mode) {
		getInstance()->button_callback(win, button, action, mode);
	};
	glfwSetMouseButtonCallback(window.getWindow(), cursor_button);

	auto window_size = [](GLFWwindow* win, int w, int h) {
		getInstance()->window_size_callback(window.getWindow(), w, h);
	};
	glfwSetWindowSizeCallback(window.getWindow(), window_size);
	glfwSetFramebufferSizeCallback(window.getWindow(), window_size);

	//glfwSetErrorCallback(error_callback);
	//glfwSetKeyCallback(window, key_callback);
	//glfwSetWindowFocusCallback(window, window_focus_callback);
	//glfwSetWindowIconifyCallback(window, window_iconify_callback);
	//glfwSetWindowSizeCallback(window, window_size_callback);
	//glfwSetCursorPosCallback(window, cursor_callback);
	//glfwSetMouseButtonCallback(window, button_callback);
}