#version 330
out vec4 FragColor;
in vec3 Normal; // Normalne s¹ nam potrzebne do odró¿nienia œcian

uniform float alfa;
struct Material { vec3 diffuse; };
uniform Material material;

void main() {
    vec3 col = material.diffuse;
    
    // Sprawdzamy, w któr¹ stronê patrzy œciana:
    if (abs(Normal.y) > 0.9) {
        // Górna œciana - zostawiamy oryginalny kolor
        FragColor = vec4(col, alfa);
    } else if (abs(Normal.x) > 0.9) {
        // Œciany boczne (lewo/prawo) - lekko przyciemniamy
        FragColor = vec4(col * 0.8, alfa);
    } else {
        // Œciany przód/ty³ - mocniej przyciemniamy
        FragColor = vec4(col * 0.6, alfa);
    }
}