//Include custom headers
#include "Application.h"

int main(void)
{
	srand(time(NULL));

	Application* app = Application::getInstance();
	app->init();
	app->loop();
}