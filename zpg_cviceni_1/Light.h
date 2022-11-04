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

enum LightType {
    Ambient = 0,
    Point = 1,
    Directional = 2,
    Spotlight = 3
};

class Light
{
public:
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 direction;
    int type;
    float cutoff = 0.0f;
    float outer_cutoff = 0.0f;

    Light() {};
    Light(int type);
    void setColor(glm::vec3 color);
    void setPosition(glm::vec3 pos);
    void setDirection(glm::vec3 dir);
    void setCutoff(float f);
    void setOuterCutoff(float f);
};

