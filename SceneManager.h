#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class SceneManager {
public:
    void drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, int size, int steps );
    void drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view );
};

#endif