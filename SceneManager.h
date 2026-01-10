#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

class SceneManager {
public:
    // Rysuje siatkę na tylnych ścianach (backX, backZ)
    void drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, int step, float backX, float backZ );

    // Rysuje ramki, oraz podpisy na PRZEDNICH krawędziach (frontX, frontZ)
    void drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, float maxVal,
        float backX, float backZ, float frontX, float frontZ );

    void drawString( float x, float y, float z, const std::string& text );
};

#endif