#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cfloat> 
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "shaderLoader.h"
#include "DataLoader.h"
#include "SceneManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


/*
Scieżki do plikow

"DANE/dane.txt"
"DANE/Gielda.txt"       - duzo kolumn
"DANE/Klimat_ekstremalny.txt"       - duzo kolumn
"DANE/Matematyka_fale.txt"      - duzo kolumn
"DANE/Stress_Test.txt"      - malo kolumn

"DANE/Zuzycie_energi.txt"       - malo kolumn
"DANE/Ruch_strony.txt"      - malo kolumn
"DANE/Sprzedarz.txt"      - malo kolumn

*/

// --- KONFIGURACJA ---
const std::string DATA_PATH = "DANE/dane_histogram.txt";

enum ChartType {
    CHART_BAR,
    CHART_HISTOGRAM
};

const ChartType CURRENT_CHART_TYPE = CHART_HISTOGRAM;

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
unsigned int lineVAO, lineVBO;

const float CHART_WIDTH = 600.0f;
const float CHART_HEIGHT = 400.0f;
const float CHART_DEPTH = 600.0f;

std::vector<glm::vec3> seriesColors = {
    glm::vec3( 0.9f, 0.9f, 0.2f ),
    glm::vec3( 0.2f, 0.2f, 0.9f ),
    glm::vec3( 0.2f, 0.7f, 0.2f ),
    glm::vec3( 0.9f, 0.2f, 0.2f )
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

    float lines[] = {
        -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f,
        -0.5f, 1.0f, -0.5f, 0.5f, 1.0f, -0.5f, 0.5f, 1.0f, -0.5f, 0.5f, 1.0f, 0.5f,
        0.5f, 1.0f, 0.5f, -0.5f, 1.0f, 0.5f, -0.5f, 1.0f, 0.5f, -0.5f, 1.0f, -0.5f,
        -0.5f, 0.0f, -0.5f, -0.5f, 1.0f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 1.0f, -0.5f,
        0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 1.0f, 0.5f
    };
    glGenVertexArrays( 1, &lineVAO ); glGenBuffers( 1, &lineVBO );
    glBindVertexArray( lineVAO ); glBindBuffer( GL_ARRAY_BUFFER, lineVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( lines ), lines, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
}

void rysuj( void ) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glm::mat4 Projection = glm::perspective( glm::radians( 45.0f ), (float)screen_width / screen_height, 0.1f, 5000.0f );
    glm::mat4 View = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 0, kameraD ) );
    View = glm::rotate( View, (float)glm::radians( kameraZ ), glm::vec3( 1, 0, 0 ) );
    View = glm::rotate( View, (float)glm::radians( kameraX ), glm::vec3( 0, 1, 0 ) );
    View = glm::translate( View, glm::vec3( -CHART_WIDTH / 2.0f, -CHART_HEIGHT / 2.0f, -CHART_DEPTH / 2.0f ) );

    glm::vec4 corners[ 4 ] = {
        glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ),
        glm::vec4( CHART_WIDTH, 0.0f, 0.0f, 1.0f ),
        glm::vec4( CHART_WIDTH, 0.0f, CHART_DEPTH, 1.0f ),
        glm::vec4( 0.0f, 0.0f, CHART_DEPTH, 1.0f )
    };

    float maxZ = -FLT_MAX; float minZ = FLT_MAX;
    int nearestIdx = 0; int farthestIdx = 0;

    for( int i = 0; i < 4; i++ ) {
        glm::vec4 viewPos = View * corners[ i ];
        if( viewPos.z > maxZ ) { maxZ = viewPos.z; nearestIdx = i; }
        if( viewPos.z < minZ ) { minZ = viewPos.z; farthestIdx = i; }
    }

    float backX = corners[ farthestIdx ].x;
    float backZ = corners[ farthestIdx ].z;
    float frontX = corners[ nearestIdx ].x;
    float frontZ = corners[ nearestIdx ].z;

    glUseProgram( programID );
    glUniform3f( materialdiffuse_id, 0.85f, 0.85f, 0.85f );
    scena.drawGrid( MVP_id, Projection, View, CHART_WIDTH, CHART_HEIGHT, CHART_DEPTH, 50, backX, backZ );
    glUniform3f( materialdiffuse_id, 0.0f, 0.0f, 0.0f );
    scena.drawAxes( MVP_id, Projection, View, CHART_WIDTH, CHART_HEIGHT, CHART_DEPTH, daneProjektu.getMaxValue(), backX, backZ, frontX, frontZ );

    glUseProgram( programID );
    float numSeries = (float)daneProjektu.allData.size();
    std::vector<std::string> seriesNames;

    if( numSeries > 0 ) {
        float numCols = (float)daneProjektu.allData[ 0 ].values.size();
        float maxVal = daneProjektu.getMaxValue();
        if( maxVal < 1.0f ) maxVal = 1.0f;

        float stepZ = CHART_DEPTH / numSeries;
        float stepX = CHART_WIDTH / numCols;
        glBindVertexArray( cubeVAO );

        for( size_t i = 0; i < daneProjektu.allData.size(); i++ ) {
            seriesNames.push_back( daneProjektu.allData[ i ].label );
            glm::vec3 col = seriesColors[ i % seriesColors.size() ];
            float zPos = i * stepZ + stepZ / 2.0f;

            if( CURRENT_CHART_TYPE == CHART_HISTOGRAM ) {
                float barWidth = stepX;
                float barDepth = 10.0f;
                for( size_t j = 0; j < daneProjektu.allData[ i ].values.size(); j++ ) {
                    float val = daneProjektu.allData[ i ].values[ j ];
                    float h = (val / maxVal) * CHART_HEIGHT; if( h < 0.1f ) h = 0.1f;
                    float posX = j * stepX + stepX / 2.0f;
                    glm::mat4 Model = glm::translate( glm::mat4( 1.0f ), glm::vec3( posX, 0.0f, zPos ) );
                    Model = glm::scale( Model, glm::vec3( barWidth, h, barDepth ) );
                    glm::mat4 MVP_final = Projection * View * Model;
                    glUniformMatrix4fv( MVP_id, 1, GL_FALSE, &MVP_final[ 0 ][ 0 ] );
                    glUniformMatrix4fv( model_id, 1, GL_FALSE, &Model[ 0 ][ 0 ] );
                    glBindVertexArray( cubeVAO );
                    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    glUniform3f( materialdiffuse_id, col.r, col.g, col.b );
                    glDrawArrays( GL_TRIANGLES, 0, 36 );
                    glBindVertexArray( lineVAO );
                    glLineWidth( 1.0f );
                    glUniform3f( materialdiffuse_id, 0.0f, 0.0f, 0.0f );
                    glDrawArrays( GL_LINES, 0, 24 );
                }
            }
            else {
                float barWidth = stepX * 0.7f;
                float barDepth = stepZ * 0.7f;
                for( size_t j = 0; j < daneProjektu.allData[ i ].values.size(); j++ ) {
                    float val = daneProjektu.allData[ i ].values[ j ];
                    float h = (val / maxVal) * CHART_HEIGHT; if( h < 0.1f ) h = 0.1f;
                    float posX = j * stepX + stepX / 2.0f;
                    glm::mat4 Model = glm::translate( glm::mat4( 1.0f ), glm::vec3( posX, 0.0f, zPos ) );
                    Model = glm::scale( Model, glm::vec3( barWidth, h, barDepth ) );
                    glm::mat4 MVP_final = Projection * View * Model;
                    glUniformMatrix4fv( MVP_id, 1, GL_FALSE, &MVP_final[ 0 ][ 0 ] );
                    glUniformMatrix4fv( model_id, 1, GL_FALSE, &Model[ 0 ][ 0 ] );
                    glBindVertexArray( cubeVAO );
                    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    glUniform3f( materialdiffuse_id, col.r, col.g, col.b );
                    glDrawArrays( GL_TRIANGLES, 0, 36 );
                    glBindVertexArray( lineVAO );
                    glLineWidth( 2.0f );
                    glUniform3f( materialdiffuse_id, 0.0f, 0.0f, 0.0f );
                    glDrawArrays( GL_LINES, 0, 24 );
                }
            }
        }
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    // 3. Etykiety
    glUseProgram( 0 );
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION ); glLoadMatrixf( &Projection[ 0 ][ 0 ] );
    glMatrixMode( GL_MODELVIEW ); glLoadMatrixf( &View[ 0 ][ 0 ] );
    glColor3f( 0.0f, 0.0f, 0.0f );

    if( numSeries > 0 ) {
        float stepZ = CHART_DEPTH / numSeries;
        float stepX = CHART_WIDTH / daneProjektu.allData[ 0 ].values.size();
        float dirX = (frontX > CHART_WIDTH / 2.0f) ? 1.0f : -1.0f;
        float dirZ = (frontZ > CHART_DEPTH / 2.0f) ? 1.0f : -1.0f;

        // Podpisy Serii (Z)
        // Jeśli jest ich bardzo dużo (>10), podpisujemy tylko co 2-gą
        int skipZ = (numSeries > 10) ? 2 : 1;
        for( size_t i = 0; i < daneProjektu.allData.size(); i += skipZ ) {
            float zPos = i * stepZ + stepZ / 2.0f;
            float tx = frontX + (dirX * 40.0f);
            if( dirX < 0 ) tx -= 50.0f;
            scena.drawString( tx, 0.0f, zPos, daneProjektu.allData[ i ].label );
        }
        // Tytuł osi Z
        float titleZ_X = frontX + (dirX * 100.0f);
        if( dirX < 0 ) titleZ_X -= 60.0f;
        scena.drawString( titleZ_X, -50.0f, CHART_DEPTH / 2.0f, "Serie (Z)" );

        // Podpisy Danych (X)
        int skipX = (CURRENT_CHART_TYPE == CHART_HISTOGRAM) ? 5 : 1;
        for( size_t j = 0; j < daneProjektu.allData[ 0 ].values.size(); j += skipX ) {
            float posX = j * stepX + stepX / 2.0f;
            std::string label = (CURRENT_CHART_TYPE == CHART_HISTOGRAM) ? std::to_string( j ) : "D" + std::to_string( j + 1 );
            float tz = frontZ + (dirZ * 40.0f);
            scena.drawString( posX - 10.0f, 0, tz, label );
        }
    }

    scena.drawLegend( screen_width, screen_height, seriesNames, seriesColors );
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

    if( !daneProjektu.load( DATA_PATH ) ) return 1;
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