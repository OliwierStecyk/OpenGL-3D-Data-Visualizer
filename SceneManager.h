#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>

class SceneManager {
public:
    void drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, int step, float backX, float backZ );
    void drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, float maxVal, float backX, float backZ, float frontX, float frontZ );
    void drawString( float x, float y, float z, const std::string& text );

    // NOWA METODA: Rysowanie legendy 2D
    void drawLegend( int screenW, int screenH, const std::vector<std::string>& labels, const std::vector<glm::vec3>& colors );
};

#endif