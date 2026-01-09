#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include <vector>
#include "glm/glm.hpp"

namespace ColorPalette {

    // 1. Paleta pastelowa (podobna do Twojego obrazka)
    const std::vector<glm::vec3> PASTEL = {
        glm::vec3( 1.0f, 0.4f, 0.6f ), // Ró¿owy
        glm::vec3( 0.6f, 0.5f, 1.0f ), // Fioletowy
        glm::vec3( 0.4f, 0.9f, 1.0f ), // B³êkitny
        glm::vec3( 0.5f, 1.0f, 0.8f ), // Miêtowy
        glm::vec3( 1.0f, 0.9f, 0.5f )  // ¯ó³tawy
    };

    // 2. Paleta "Modern/Flat"
    const std::vector<glm::vec3> MODERN = {
        glm::vec3( 0.18f, 0.5f, 0.92f ), // Niebieski
        glm::vec3( 0.1f, 0.73f, 0.61f ),  // Szmaragdowy
        glm::vec3( 0.9f, 0.3f, 0.23f ),  // Czerwony
        glm::vec3( 0.6f, 0.35f, 0.7f ),  // Fioletowy
        glm::vec3( 0.95f, 0.6f, 0.1f )   // Pomarañczowy
    };

    // 3. Paleta "Heatmap" (od niebieskiego do czerwonego)
    const std::vector<glm::vec3> HEATMAP = {
        glm::vec3( 0.2f, 0.2f, 1.0f ), // Zimny
        glm::vec3( 0.4f, 0.6f, 1.0f ),
        glm::vec3( 1.0f, 0.8f, 0.2f ),
        glm::vec3( 1.0f, 0.4f, 0.0f ),
        glm::vec3( 1.0f, 0.0f, 0.0f )  // Gor¹cy
    };

    // Funkcja pomocnicza, która wybiera kolor z wybranej palety
    // i dba o to, ¿eby indeks nie wyszed³ poza zakres (u¿ywa modulo)
    inline glm::vec3 getColor( int index, const std::vector<glm::vec3>& palette ) {
        return palette[ index % palette.size() ];
    }
}

#endif