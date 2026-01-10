#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cfloat> // dla FLT_MAX
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
double kameraX = 25.0, kameraZ = -20.0, kameraD = -1600.0;
int pozycjaMyszyX, pozycjaMyszyY, mbutton;
double popX, popZ, popD;

// Shadery
GLuint programID, MVP_id, model_id, materialdiffuse_id, materialambient_id, materialspecular_id, materialshine_id, lightColor_id, lightPos_id;
unsigned int cubeVAO, cubeVBO;

const float CHART_WIDTH = 600.0f;
const float CHART_HEIGHT = 400.0f;
const float CHART_DEPTH = 600.0f;

glm::vec3 seriesColors[] = {
    glm::vec3( 1.0f, 0.4f, 0.6f ),
    glm::vec3( 0.6f, 0.5f, 1.0f ),
    glm::vec3( 0.4f, 0.9f, 1.0f ),
    glm::vec3( 0.5f, 1.0f, 0.5f )
};

void setupCube() {
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

    // Obliczanie macierzy widoku
    glm::mat4 Projection = glm::perspective( glm::radians( 45.0f ), (float)screen_width / screen_height, 0.1f, 5000.0f );

    glm::mat4 View = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 0, kameraD ) );
    View = glm::rotate( View, (float)glm::radians( kameraZ ), glm::vec3( 1, 0, 0 ) );
    View = glm::rotate( View, (float)glm::radians( kameraX ), glm::vec3( 0, 1, 0 ) );
    // Przesuwamy centrum świata do środka wykresu
    View = glm::translate( View, glm::vec3( -CHART_WIDTH / 2.0f, -CHART_HEIGHT / 2.0f, -CHART_DEPTH / 2.0f ) );

    // --- ALGORYTM SORTOWANIA NAROŻNIKÓW ---
    // Definiujemy 4 narożniki podstawy (Y=0)
    glm::vec4 corners[ 4 ] = {
        glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ),            // 0: Lewy-Tył
        glm::vec4( CHART_WIDTH, 0.0f, 0.0f, 1.0f ),     // 1: Prawy-Tył
        glm::vec4( CHART_WIDTH, 0.0f, CHART_DEPTH, 1.0f ), // 2: Prawy-Przód
        glm::vec4( 0.0f, 0.0f, CHART_DEPTH, 1.0f )      // 3: Lewy-Przód
    };

    // Transformujemy punkty do przestrzeni widoku (View Space), aby sprawdzić ich głębokość (Z)
    float maxZ = -FLT_MAX; // Najbliżej kamery (w OpenGL View Space: im większe Z, tym bliżej - uwaga na konwencję)
    float minZ = FLT_MAX;  // Najdalej od kamery

    int nearestIdx = 0;
    int farthestIdx = 0;

    for( int i = 0; i < 4; i++ ) {
        glm::vec4 viewPos = View * corners[ i ];
        // W View Space: kamera jest w (0,0,0) patrząc w -Z.
        // Im większa wartość Z (bliższa zeru lub dodatnia), tym bliżej kamery.

        if( viewPos.z > maxZ ) {
            maxZ = viewPos.z;
            nearestIdx = i;
        }
        if( viewPos.z < minZ ) {
            minZ = viewPos.z;
            farthestIdx = i;
        }
    }

    // Pobieramy współrzędne najdalszego rogu (do siatki tła)
    float backX = corners[ farthestIdx ].x;
    float backZ = corners[ farthestIdx ].z;

    // Pobieramy współrzędne najbliższego rogu (do podpisów osi)
    float frontX = corners[ nearestIdx ].x;
    float frontZ = corners[ nearestIdx ].z;

    // --------------------------------------

    // 1. Rysowanie środowiska
    glUseProgram( programID );

    // Siatkę (ściany) rysujemy zawsze w narożniku najdalszym (backX, backZ)
    scena.drawGrid( MVP_id, Projection, View, CHART_WIDTH, CHART_HEIGHT, CHART_DEPTH, 50, backX, backZ );

    // Osie i podpisy rysujemy względem obu punktów (ramka łączy back i front)
    scena.drawAxes( MVP_id, Projection, View, CHART_WIDTH, CHART_HEIGHT, CHART_DEPTH, daneProjektu.getMaxValue(),
        backX, backZ, frontX, frontZ );

    // 2. Rysowanie słupków
    glUseProgram( programID );
    float numSeries = (float)daneProjektu.allData.size();
    if( numSeries > 0 ) {
        float numCols = (float)daneProjektu.allData[ 0 ].values.size();
        float maxVal = daneProjektu.getMaxValue();
        if( maxVal < 1.0f ) maxVal = 1.0f;

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
                if( h < 0.1f ) h = 0.1f;

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

    // 3. Etykiety (Text)
    glUseProgram( 0 );
    glDisable( GL_DEPTH_TEST ); // Napisy zawsze na wierzchu

    glMatrixMode( GL_PROJECTION ); glLoadMatrixf( &Projection[ 0 ][ 0 ] );
    glMatrixMode( GL_MODELVIEW ); glLoadMatrixf( &View[ 0 ][ 0 ] );
    glColor3f( 0.0f, 0.0f, 0.0f );

    if( numSeries > 0 ) {
        float stepZ = CHART_DEPTH / numSeries;
        float stepX = CHART_WIDTH / daneProjektu.allData[ 0 ].values.size();

        // Obliczamy wektory "na zewnątrz" od środka wykresu
        // Środek wykresu to (W/2, 0, D/2)
        // Kierunek X = (frontX - W/2) -> znormalizowany znak
        float dirX = (frontX > CHART_WIDTH / 2.0f) ? 1.0f : -1.0f;
        float dirZ = (frontZ > CHART_DEPTH / 2.0f) ? 1.0f : -1.0f;

        // 1. Etykiety Serii (Z)
        // Rysujemy je wzdłuż krawędzi frontX
        for( size_t i = 0; i < daneProjektu.allData.size(); i++ ) {
            float zPos = i * stepZ + stepZ / 2.0f;
            float tx = frontX + (dirX * 40.0f);
            if( dirX < 0 ) tx -= 50.0f; // Korekta wyrównania tekstu
            scena.drawString( tx, 0.0f, zPos, daneProjektu.allData[ i ].label );
        }

        // 2. Etykiety Kolumn (X)
        // Rysujemy je wzdłuż krawędzi frontZ
        for( size_t j = 0; j < daneProjektu.allData[ 0 ].values.size(); j++ ) {
            float posX = j * stepX + stepX / 2.0f;
            std::string label = "D" + std::to_string( j + 1 );
            float tz = frontZ + (dirZ * 40.0f);
            scena.drawString( posX - 10.0f, 0, tz, label );
        }
    }

    glEnable( GL_DEPTH_TEST );
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
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

    if( !daneProjektu.load( "dane.txt" ) ) return 1;
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