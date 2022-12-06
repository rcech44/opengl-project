//Include custom headers
#include "Application.h"

int main(void)
{
	Application* app = Application::getInstance();
	app->init();
	app->loop();
}