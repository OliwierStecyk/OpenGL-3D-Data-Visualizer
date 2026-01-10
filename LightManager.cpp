#include "LightManager.h"
#include <string>

void LightManager::setupDefaultSceneLights() {
    lights.clear();

    // Œwiat³o 1: Przód - Góra - Prawo
    lights.push_back( { glm::vec3( 1000.0f, 1000.0f, 1000.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ) } );

    // Œwiat³o 2: Ty³ - Góra - Lewo (oœwietla to co z ty³u)
    lights.push_back( { glm::vec3( -500.0f, 800.0f, -500.0f ), glm::vec3( 0.7f, 0.7f, 0.8f ) } );

    // Œwiat³o 3: Przód - Dó³ - Lewo (rozjaœnia cienie na dole)
    lights.push_back( { glm::vec3( -200.0f, -200.0f, 500.0f ), glm::vec3( 0.4f, 0.4f, 0.5f ) } );

    // Œwiat³o 4: Z boku (mocno rozproszone)
    lights.push_back( { glm::vec3( 1000.0f, 200.0f, -200.0f ), glm::vec3( 0.5f, 0.5f, 0.5f ) } );
}

void LightManager::applyLights( GLuint shaderProgram ) {
    int count = ( int ) lights.size();
    glUniform1i( glGetUniformLocation( shaderProgram, "numLights" ), count );

    for( int i = 0; i < count; i++ ) {
        std::string posName = "allLights[" + std::to_string( i ) + "].position";
        std::string colName = "allLights[" + std::to_string( i ) + "].color";

        glUniform3fv( glGetUniformLocation( shaderProgram, posName.c_str() ), 1, &lights[ i ].position[ 0 ] );
        glUniform3fv( glGetUniformLocation( shaderProgram, colName.c_str() ), 1, &lights[ i ].color[ 0 ] );
    }
}