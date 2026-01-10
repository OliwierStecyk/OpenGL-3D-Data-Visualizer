#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <vector>
#include <string> // Wa¿ne dla std::string

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

class LightManager {
public:
    std::vector<Light> lights;

    // Ustawia œwiat³a wokó³ wykresu
    void setupDefaultSceneLights();

    // Wysy³a dane oœwietlenia do shadera
    void applyLights( GLuint shaderProgram );
};

#endif