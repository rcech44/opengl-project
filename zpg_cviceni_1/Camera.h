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
#include "Constants.h"
#include "Observer.h"

class Camera : public Observer, public Observable
{
private:
    glm::vec3 eye{ 0.f, 0.f, 4.0f };
    glm::vec3 target{ 0.f, 0.f, -1.f };
    glm::vec3 up{ 0.f, 1.0f, 0.f };
    glm::mat4 camera;
    glm::mat4 projectMatrix = glm::perspective(glm::radians(80.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 200.0f);

    bool changed = true;
    float cameraMoveMultiplier = 15.f;
    float cameraYMoveMultiplier = 30.f;

public:
    Camera();
    void setPosition(glm::vec3 position);

    // movement
    void moveForward(Direction dir, double time);
    void moveSideways(Direction dir, double time);
    void moveHeight(Direction dir, double time);

    void apply();
    glm::mat4 getView();
    glm::mat4 getProjection();
    glm::vec3 getPosition();
    glm::vec3 getTarget();
    void setProjection(int width, int height);

    void notify(EventType eventType, void* object) override;
};

