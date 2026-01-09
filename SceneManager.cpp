#include "GL/glew.h"      // GLEW ZAWSZE musi byæ pierwszy
#include "GL/freeglut.h"  // To tutaj zdefiniowany jest GLUT_BITMAP_HELVETICA_12
#include "SceneManager.h"
#include <string>


void SceneManager::drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, int size, int steps ) {
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    glBegin( GL_LINES );
    glColor3f( 0.7f, 0.7f, 0.7f ); // Jasnoszary kolor linii

    // 1. Pod³oga (p³aszczyzna XZ)
    for( int i = 0; i <= size; i += steps ) {
        glVertex3f( ( float ) i, 0, 0 ); glVertex3f( ( float ) i, 0, ( float ) size );
        glVertex3f( 0, 0, ( float ) i ); glVertex3f( ( float ) size, 0, ( float ) i );
    }

    // 2. Lewa œciana (p³aszczyzna ZY przy X=0)
    for( int i = 0; i <= size; i += steps ) {
        glVertex3f( 0, 0, ( float ) i ); glVertex3f( 0, ( float ) size / 2, ( float ) i ); // pionowe
        glVertex3f( 0, ( float ) i / 2, 0 ); glVertex3f( 0, ( float ) i / 2, ( float ) size ); // poziome
    }

    // 3. Tylna œciana (p³aszczyzna XY przy Z=0)
    for( int i = 0; i <= size; i += steps ) {
        glVertex3f( ( float ) i, 0, 0 ); glVertex3f( ( float ) i, ( float ) size / 2, 0 ); // pionowe
        glVertex3f( 0, ( float ) i / 2, 0 ); glVertex3f( ( float ) size, ( float ) i / 2, 0 ); // poziome
    }
    glEnd();
}

void SceneManager::drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view ) {
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    glLineWidth( 2.0f );
    glBegin( GL_LINES );
    // Wszystkie osie na czarno lub ciemnoszaro dla stylu "paper"
    glColor3f( 0.0f, 0.0f, 0.0f );

    // Podstawowe krawêdzie pude³ka
    glVertex3f( 0, 0, 0 ); glVertex3f( 600, 0, 0 );   // Oœ X
    glVertex3f( 0, 0, 0 ); glVertex3f( 0, 400, 0 );   // Oœ Y
    glVertex3f( 0, 0, 0 ); glVertex3f( 0, 0, 600 );   // Oœ Z

    // Zamkniêcie góry pude³ka (opcjonalne, dla lepszego efektu 3D)
    glVertex3f( 600, 0, 0 ); glVertex3f( 600, 0, 600 );
    glVertex3f( 0, 0, 600 ); glVertex3f( 600, 0, 600 );
    glEnd();
    glLineWidth( 1.0f );
}