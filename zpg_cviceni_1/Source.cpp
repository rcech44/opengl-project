//Include GLEW
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>  

//Include GLM  
//#include <glm/vec3.hpp> // glm::vec3
//#include <glm/vec4.hpp> // glm::vec4
//#include <glm/mat4x4.hpp> // glm::mat4
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>

//Include custom headers
#include "Application.h"

// Application - Scena (obsahuje kres. objekt) - Kreslitelny objekt (obsahuje Shader a Model s VAOVBO) - Model a Shader vytvorit
// Scena = Kres. objekty, while ()
// Kreslitelny objekt = Model a Shader, nastavSe(), transformuj()
// Shader = int ShaderProgram, nastavSe(), nastat id matice
// Model = int VAO, int Type, int pocetBodu
// (Composite) Transformace = obsahuje matici a metody (nastav pozici, rotuj, zmen velikost)

int main(void)
{
	Application* app = Application::getInstance();
	Application* app2 = Application::getInstance();
	app->init();
	app->loop();
}