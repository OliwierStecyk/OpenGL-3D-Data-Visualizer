#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

class SceneManager {
public:
    // Rysuje siatkę o wymiarach w, h, d
    void drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, int step );

    // Rysuje osie, pudełko (bez góry) i podstawowe podpisy osi (X, Y, Z)
    void drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, float maxVal );

    // Pomocnicza funkcja do rysowania tekstu w przestrzeni 3D
    void drawString( float x, float y, float z, const std::string& text );
};

#endif