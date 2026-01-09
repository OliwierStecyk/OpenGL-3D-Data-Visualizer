#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "shaderLoader.h"
#include "DataLoader.h"
#include "SceneManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Obiekty
DataLoader daneProjektu;
SceneManager scena;

// Okno i Kamera
int screen_width = 1024, screen_height = 768;
// Ustawienie kamery (zoom, obrót)
double kameraX = 25.0, kameraZ = -20.0, kameraD = -1600.0;
int pozycjaMyszyX, pozycjaMyszyY, mbutton;
double popX, popZ, popD;

// Shadery
GLuint programID, MVP_id, model_id, materialdiffuse_id, materialambient_id, materialspecular_id, materialshine_id, lightColor_id, lightPos_id;
unsigned int cubeVAO, cubeVBO;

// Wymiary przestrzeni wykresu
const float CHART_WIDTH = 600.0f; // X
const float CHART_HEIGHT = 400.0f; // Y
const float CHART_DEPTH = 600.0f; // Z

// Kolory dla kolejnych serii danych
glm::vec3 seriesColors[] = {
    glm::vec3( 1.0f, 0.4f, 0.6f ), // Różowy
    glm::vec3( 0.6f, 0.5f, 1.0f ), // Fioletowy
    glm::vec3( 0.4f, 0.9f, 1.0f ), // Błękitny
    glm::vec3( 0.5f, 1.0f, 0.5f )  // Zielony
};

void setupCube() {
    // Sześcian jednostkowy z punktem odniesienia (pivotem) na dole (y=0)
    // Dzięki temu skalowanie wysokości odbywa się tylko w górę
    float v[] = {
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 1.0f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 1.0f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 1.0f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 1.0f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.0f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 1.0f, 0.5f, -1.0f, 0.0f, 0.0f,
        0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f
    };
    glGenVertexArrays( 1, &cubeVAO ); glGenBuffers( 1, &cubeVBO );
    glBindVertexArray( cubeVAO ); glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( v ), v, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)0 ); glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)(3 * sizeof( float )) ); glEnableVertexAttribArray( 1 );
}

void rysuj( void ) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // 1. Projekcja
    glm::mat4 Projection = glm::perspective( glm::radians( 45.0f ), (float)screen_width / screen_height, 0.1f, 5000.0f );

    // 2. Widok (Kamera) z centrowaniem
    glm::mat4 View = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 0, kameraD ) ); // Zoom
    View = glm::rotate( View, (float)glm::radians( kameraZ ), glm::vec3( 1, 0, 0 ) ); // Obrót X
    View = glm::rotate( View, (float)glm::radians( kameraX ), glm::vec3( 0, 1, 0 ) ); // Obrót Y

    // Przesuwamy cały świat tak, aby środek wykresu (width/2, height/2, depth/2)
    // znalazł się w (0,0,0). Dzięki temu obracamy się wokół środka wykresu.
    View = glm::translate( View, glm::vec3( -CHART_WIDTH / 2.0f, -CHART_HEIGHT / 2.0f, -CHART_DEPTH / 2.0f ) );

    // 3. Rysowanie środowiska (siatka, osie, pudełko)
    glUseProgram( programID );
    scena.drawGrid( MVP_id, Projection, View, CHART_WIDTH, CHART_HEIGHT, CHART_DEPTH, 50 );
    scena.drawAxes( MVP_id, Projection, View, CHART_WIDTH, CHART_HEIGHT, CHART_DEPTH, daneProjektu.getMaxValue() );

    // 4. Rysowanie słupków
    glUseProgram( programID );

    float numSeries = (float)daneProjektu.allData.size();
    if( numSeries > 0 ) {
        float numCols = (float)daneProjektu.allData[ 0 ].values.size();
        float maxVal = daneProjektu.getMaxValue();
        if( maxVal < 1.0f ) maxVal = 1.0f;

        // Obliczamy rozmiary słupków i odstępów
        float stepZ = CHART_DEPTH / numSeries;
        float stepX = CHART_WIDTH / numCols;
        float barWidth = stepX * 0.7f;
        float barDepth = stepZ * 0.7f;

        glBindVertexArray( cubeVAO );

        for( size_t i = 0; i < daneProjektu.allData.size(); i++ ) {
            glm::vec3 col = seriesColors[ i % 4 ];
            glUniform3f( materialdiffuse_id, col.r, col.g, col.b );

            float zPos = i * stepZ + stepZ / 2.0f;

            for( size_t j = 0; j < daneProjektu.allData[ i ].values.size(); j++ ) {
                float val = daneProjektu.allData[ i ].values[ j ];
                float h = (val / maxVal) * CHART_HEIGHT;
                if( h < 0.1f ) h = 0.1f; // Minimalna wysokość, żeby słupek nie zniknął

                float posX = j * stepX + stepX / 2.0f;

                glm::mat4 Model = glm::translate( glm::mat4( 1.0f ), glm::vec3( posX, 0.0f, zPos ) );
                Model = glm::scale( Model, glm::vec3( barWidth, h, barDepth ) );

                glm::mat4 MVP_final = Projection * View * Model;
                glUniformMatrix4fv( MVP_id, 1, GL_FALSE, &MVP_final[ 0 ][ 0 ] );
                glUniformMatrix4fv( model_id, 1, GL_FALSE, &Model[ 0 ][ 0 ] );
                glDrawArrays( GL_TRIANGLES, 0, 36 );
            }
        }
    }

    // 5. Rysowanie etykiet (tekst)
    // Wyłączamy shader, używamy standardowego potoku OpenGL dla glutBitmapCharacter
    glUseProgram( 0 );
    glMatrixMode( GL_PROJECTION ); glLoadMatrixf( &Projection[ 0 ][ 0 ] );
    glMatrixMode( GL_MODELVIEW ); glLoadMatrixf( &View[ 0 ][ 0 ] );
    glColor3f( 0.0f, 0.0f, 0.0f ); // Czarny kolor tekstu

    if( numSeries > 0 ) {
        float stepZ = CHART_DEPTH / numSeries;
        float stepX = CHART_WIDTH / daneProjektu.allData[ 0 ].values.size();

        // Pętla dla etykiet serii (Oś Z)
        for( size_t i = 0; i < daneProjektu.allData.size(); i++ ) {
            float zPos = i * stepZ + stepZ / 2.0f;

            // Podpis z LEWEJ strony
            scena.drawString( -80.0f, 0.0f, zPos, daneProjektu.allData[ i ].label );

            // Podpis z PRAWEJ strony (Nowość)
            scena.drawString( CHART_WIDTH + 20.0f, 0.0f, zPos, daneProjektu.allData[ i ].label );
        }

        // Pętla dla etykiet kolumn (Oś X)
        for( size_t j = 0; j < daneProjektu.allData[ 0 ].values.size(); j++ ) {
            float posX = j * stepX + stepX / 2.0f;
            std::string label = "D" + std::to_string( j + 1 );
            scena.drawString( posX - 10.0f, -40.0f, CHART_DEPTH + 10.0f, label );
        }
    }

    glutSwapBuffers();
}

void mysz( int b, int s, int x, int y ) { mbutton = b; if( s == GLUT_DOWN ) { pozycjaMyszyX = x; pozycjaMyszyY = y; popX = kameraX; popZ = kameraZ; popD = kameraD; } }
void ruch( int x, int y ) {
    if( mbutton == GLUT_LEFT_BUTTON ) { kameraX = popX + (x - pozycjaMyszyX) * 0.2; kameraZ = popZ + (y - pozycjaMyszyY) * 0.2; }
    if( mbutton == GLUT_RIGHT_BUTTON ) { kameraD = popD + (y - pozycjaMyszyY) * 2.0; }
    glutPostRedisplay();
}

void rozmiar( int w, int h ) { screen_width = w; screen_height = h; glViewport( 0, 0, w, h ); }
void klawisz( GLubyte key, int x, int y ) { if( key == 27 ) exit( 0 ); }

int main( int argc, char** argv ) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( screen_width, screen_height );
    glutCreateWindow( "Wizualizacja 3D" );

    glewInit();
    glEnable( GL_DEPTH_TEST );

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f ); // Białe tło

    if( !daneProjektu.load( "dane.txt" ) ) {
        printf( "Brak pliku dane.txt!\n" );
        return 1;
    }
    programID = loadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    MVP_id = glGetUniformLocation( programID, "MVP" );
    model_id = glGetUniformLocation( programID, "model" );
    materialdiffuse_id = glGetUniformLocation( programID, "material.diffuse" );
    materialambient_id = glGetUniformLocation( programID, "material.ambient" );
    materialspecular_id = glGetUniformLocation( programID, "material.specular" );
    materialshine_id = glGetUniformLocation( programID, "material.shininess" );
    lightColor_id = glGetUniformLocation( programID, "lightColor" );
    lightPos_id = glGetUniformLocation( programID, "lightPos" );

    glUseProgram( programID );
    glUniform3f( lightColor_id, 1, 1, 1 );
    glUniform3f( lightPos_id, 300, 800, 800 );
    glUniform3f( materialambient_id, 0.3f, 0.3f, 0.3f );
    glUniform3f( materialspecular_id, 0.5f, 0.5f, 0.5f );
    glUniform1f( materialshine_id, 32.0f );
    glUniform1f( glGetUniformLocation( programID, "alfa" ), 1.0f );

    setupCube();
    glutDisplayFunc( rysuj );
    glutIdleFunc( rysuj );
    glutReshapeFunc( rozmiar );
    glutMouseFunc( mysz );
    glutMotionFunc( ruch );
    glutKeyboardFunc( klawisz );
    glutMainLoop();
    return 0;
}