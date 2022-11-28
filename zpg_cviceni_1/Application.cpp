#pragma once

#include "Application.h"
#include <functional>

Window Application::window;
Application* Application::instance;

Application* Application::getInstance()
{
	// Singleton - create new instance if does not exist
	if (!instance) instance = new Application();
	return instance;
}

void Application::init()
{
	// GLFW initialization
	initGLFW();

	// Window creation and setting up cursor
	window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, "Unreal Engine 6");
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW initialization and some other OpenGL settings
	initGLEW();
	setGLSettings();

	// Create scene and initialize
	this->scene = Scene();
	this->scene.init();

	// Set callbacks
	this->setCallbacks();

	// Register camera as observer - so it can see mouse (view) changes
	mouse.registerObserver(*scene.getCamera());

	// Helpful debug info on start
	printInfo();
}

void Application::initGLFW()
{
	// GLFW initialization
	if (!glfwInit())
	{
		fprintf(stderr, "[INIT] Could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}
	else printf("[INIT] GLFW initialized successfully\n");
}

void Application::initGLEW()
{
	// GLEW initialization and enabling experimental features
	glewExperimental = GL_TRUE;
	glewInit();
}

void Application::setGLSettings()
{
	// Enable depth testing - frame (depth) buffer will contain depth information
	glEnable(GL_DEPTH_TEST);

	// Enable stencil testing - frame (stencil) buffer will contain object ID information
	glEnable(GL_STENCIL_TEST);

	// Stencil buffer settings - on error keep previous, on success replace values
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Enable blending - rendering transparent images
	glEnable(GL_BLEND);
}

void Application::loop()
{
	// Main loop
	while (!window.shouldClose())
	{
		now_time = glfwGetTime();
		update();
	}

	// Destroy window and de-initialize GLFW
	window.deleteWindow();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Application::printInfo()
{
	// System information debug
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
	// Calculate frames per second and print it every 200 frames
	last_frame_time = glfwGetTime() - now_time;
	if (frame_count % 200 == 1) printf("[FPS-DEBUG] FPS: %.0f\n", round((float)(1 / last_frame_time)));
	frame_count++;
}

void Application::update()
{
	// Scene updating
	scene.update();

	// Process callbacks (mouse, keyboard...)
	glfwPollEvents();

	// Swap frames in frame buffer
	window.swapBuffers();

	// Calculate frames per second
	debugFPS();
}

void Application::setCallbacks()
{
	// Setting callbacks
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