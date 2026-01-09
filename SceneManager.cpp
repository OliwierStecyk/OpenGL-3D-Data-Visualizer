#include "GL/glew.h"
#include "GL/freeglut.h"
#include "SceneManager.h"
#include <string>
#include <sstream>
#include <iomanip>

// Funkcja rysująca tekst bitmapowy
void SceneManager::drawString( float x, float y, float z, const std::string& text ) {
    glRasterPos3f( x, y, z );
    for( char c : text ) {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, c );
    }
}

void SceneManager::drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, int step ) {
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    glLineWidth( 1.0f );
    glBegin( GL_LINES );
    glColor3f( 0.8f, 0.8f, 0.8f ); // Jasnoszary kolor siatki

    // 1. Podłoga (płaszczyzna XZ)
    for( float i = 0; i <= w; i += step ) { glVertex3f( i, 0, 0 ); glVertex3f( i, 0, d ); }
    for( float i = 0; i <= d; i += step ) { glVertex3f( 0, 0, i ); glVertex3f( w, 0, i ); }

    // 2. Tylna ściana (płaszczyzna XY)
    for( float i = 0; i <= w; i += step ) { glVertex3f( i, 0, 0 ); glVertex3f( i, h, 0 ); }
    for( float i = 0; i <= h; i += step ) { glVertex3f( 0, i, 0 ); glVertex3f( w, i, 0 ); }

    // 3. Lewa ściana (płaszczyzna ZY)
    for( float i = 0; i <= d; i += step ) { glVertex3f( 0, 0, i ); glVertex3f( 0, h, i ); }
    for( float i = 0; i <= h; i += step ) { glVertex3f( 0, i, 0 ); glVertex3f( 0, i, d ); }

    glEnd();
}

void SceneManager::drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, float maxVal ) {
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    // Rysowanie obramowania "pudełka" (otwartego od góry)
    glLineWidth( 2.0f );
    glBegin( GL_LINES );
    glColor3f( 0.0f, 0.0f, 0.0f ); // Czarne krawędzie

    // 1. Dolny pierścień (Podstawa)
    glVertex3f( 0, 0, 0 ); glVertex3f( w, 0, 0 );
    glVertex3f( w, 0, 0 ); glVertex3f( w, 0, d );
    glVertex3f( w, 0, d ); glVertex3f( 0, 0, d );
    glVertex3f( 0, 0, d ); glVertex3f( 0, 0, 0 );

    // 2. Pionowe krawędzie w narożnikach (Słupki)
    glVertex3f( 0, 0, 0 ); glVertex3f( 0, h, 0 ); // Lewy tył
    glVertex3f( w, 0, 0 ); glVertex3f( w, h, 0 ); // Prawy tył
    glVertex3f( w, 0, d ); glVertex3f( w, h, d ); // Prawy przód
    glVertex3f( 0, 0, d ); glVertex3f( 0, h, d ); // Lewy przód

    // UWAGA: Nie rysujemy "górnego pierścienia", żeby góra była otwarta
    glEnd();

    // Rysowanie TICK MARKS (kreski podziałki)
    glBegin( GL_LINES );
    int numTicks = 5;

    // Oś Y (Wysokość) - kreski na lewym przednim słupku
    for( int i = 0; i <= numTicks; i++ ) {
        float y = (h / numTicks) * i;
        glVertex3f( 0, y, d ); glVertex3f( -15.0f, y, d ); // Wychodzą w lewo
    }
    // Oś X (Szerokość) - kreski na dolnej krawędzi
    for( int i = 0; i <= numTicks; i++ ) {
        float x = (w / numTicks) * i;
        glVertex3f( x, 0, d ); glVertex3f( x, -15.0f, d ); // Wychodzą w dół
    }
    // Oś Z (Głębokość) - kreski z lewej strony
    for( int i = 0; i <= numTicks; i++ ) {
        float z = (d / numTicks) * i;
        glVertex3f( 0, 0, z ); glVertex3f( -15.0f, 0, z ); // Wychodzą w lewo
    }
    // Oś Z (Głębokość) - kreski z PRAWEJ strony (dla symetrii)
    for( int i = 0; i <= numTicks; i++ ) {
        float z = (d / numTicks) * i;
        glVertex3f( w, 0, z ); glVertex3f( w + 15.0f, 0, z ); // Wychodzą w prawo
    }
    glEnd();
    glLineWidth( 1.0f );

    // --- RYSOWANIE TEKSTU (Opisy osi i wartości) ---
    // Musimy wyłączyć shader, aby użyć glutBitmapString/Character
    glUseProgram( 0 );

    // Ustawiamy macierze Fixed Function Pipeline pod tekst
    glMatrixMode( GL_PROJECTION ); glLoadMatrixf( &projection[ 0 ][ 0 ] );
    glMatrixMode( GL_MODELVIEW ); glLoadMatrixf( &view[ 0 ][ 0 ] );
    glColor3f( 0.0f, 0.0f, 0.0f ); // Czarny tekst

    // Wartości liczbowe przy podziałce Y
    for( int i = 0; i <= numTicks; i++ ) {
        float y = (h / numTicks) * i;
        float val = (maxVal / numTicks) * i;

        std::stringstream ss;
        ss << std::fixed << std::setprecision( 1 ) << val;
        // Tekst przy lewym słupku, lekko odsunięty
        drawString( -60.0f, y, d + 10.0f, ss.str() );
    }

    // Główne tytuły osi
    drawString( w / 2.0f, -60.0f, d, "Osi Pomiary (X)" ); // Pod osią X
    drawString( -100.0f, h / 2.0f, d, "Wartosc (Y)" );    // Obok osi Y (Lewa)

    // Tytuł osi Z (z lewej)
    drawString( -100.0f, 0.0f, d / 2.0f, "Serie" );

    // Tytuł osi Z (z PRAWEJ)
    drawString( w + 30.0f, 0.0f, d / 2.0f, "Serie" );
}