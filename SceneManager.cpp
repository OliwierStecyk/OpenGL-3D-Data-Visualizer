#include "GL/glew.h"
#include "GL/freeglut.h"
#include "SceneManager.h"
#include <string>
#include <sstream>
#include <iomanip>

void SceneManager::drawString( float x, float y, float z, const std::string& text ) {
    glRasterPos3f( x, y, z );
    for( char c : text ) {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, c );
    }
}

void SceneManager::drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, int step, float backX, float backZ ) {
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    glLineWidth( 1.0f );
    glBegin( GL_LINES );
    glColor3f( 0.85f, 0.85f, 0.85f ); // Jasnoszary

    // 1. Podłoga (XZ)
    for( float i = 0; i <= w; i += step ) { glVertex3f( i, 0, 0 ); glVertex3f( i, 0, d ); }
    for( float i = 0; i <= d; i += step ) { glVertex3f( 0, 0, i ); glVertex3f( w, 0, i ); }

    // 2. Ściana boczna (na backX)
    for( float i = 0; i <= d; i += step ) { glVertex3f( backX, 0, i ); glVertex3f( backX, h, i ); }
    for( float i = 0; i <= h; i += step ) { glVertex3f( backX, i, 0 ); glVertex3f( backX, i, d ); }

    // 3. Ściana tylna (na backZ)
    for( float i = 0; i <= w; i += step ) { glVertex3f( i, 0, backZ ); glVertex3f( i, h, backZ ); }
    for( float i = 0; i <= h; i += step ) { glVertex3f( 0, i, backZ ); glVertex3f( w, i, backZ ); }

    glEnd();
}

void SceneManager::drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, float maxVal,
    float backX, float backZ, float frontX, float frontZ ) {
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    // Kierunki:
    // dirX/dirZ - określają kierunek "na zewnątrz" od krawędzi frontowych
    float dirX = (frontX > 0.1f) ? 1.0f : -1.0f;
    float dirZ = (frontZ > 0.1f) ? 1.0f : -1.0f;

    // dirBackX - określa, po której stronie jest "tylna" ściana (lewo/prawo).
    // Użyjemy tego, żeby wiedzieć w którą stronę wystawić tekst osi Y.
    // Jeśli backX > 0 (prawa), to tekst Y dajemy w prawo. Jeśli 0 (lewa), to w lewo.
    float dirBackX = (backX > 0.1f) ? 1.0f : -1.0f;

    glLineWidth( 2.0f );
    glBegin( GL_LINES );
    glColor3f( 0.0f, 0.0f, 0.0f ); // Czarny

    // --- RAMKA PODŁOGI ---
    glVertex3f( 0, 0, 0 ); glVertex3f( w, 0, 0 );
    glVertex3f( w, 0, 0 ); glVertex3f( w, 0, d );
    glVertex3f( w, 0, d ); glVertex3f( 0, 0, d );
    glVertex3f( 0, 0, d ); glVertex3f( 0, 0, 0 );

    // --- SŁUPKI PIONOWE ---
    // 1. Tylny kręgosłup (tam gdzie jest siatka)
    glVertex3f( backX, 0, backZ ); glVertex3f( backX, h, backZ );

    // 2. PRZEDNI SŁUPEK DLA OSI Y (Nowość)
    // Rysujemy go w narożniku [backX, frontZ]. 
    // Czyli: po tej samej stronie co siatka boczna, ale z przodu wykresu.
    glVertex3f( backX, 0, frontZ ); glVertex3f( backX, h, frontZ );

    // --- GÓRNE KRAWĘDZIE TYLNYCH ŚCIAN ---
    glVertex3f( backX, h, 0 ); glVertex3f( backX, h, d ); // Góra boczna
    glVertex3f( 0, h, backZ ); glVertex3f( w, h, backZ ); // Góra tylna

    // --- PODZIAŁKI (TICKS) ---
    int numTicks = 5;
    float tickLen = 15.0f;

    // Ticks Oś Y (Na PRZEDNIM słupku - backX, frontZ)
    for( int i = 0; i <= numTicks; i++ ) {
        float y = (h / numTicks) * i;
        glVertex3f( backX, y, frontZ );
        // Kreska wystaje w bok (na zewnątrz wykresu)
        glVertex3f( backX + (tickLen * dirBackX), y, frontZ );
    }

    // Ticks Oś X (Na przedniej krawędzi - frontZ)
    for( int i = 0; i <= numTicks; i++ ) {
        float x = (w / numTicks) * i;
        glVertex3f( x, 0, frontZ );
        glVertex3f( x, 0, frontZ + (tickLen * dirZ) ); // Wystaje do przodu
    }

    // Ticks Oś Z (Na bocznej przedniej krawędzi - frontX)
    for( int i = 0; i <= numTicks; i++ ) {
        float z = (d / numTicks) * i;
        glVertex3f( frontX, 0, z );
        glVertex3f( frontX + (tickLen * dirX), 0, z ); // Wystaje w bok
    }
    glEnd();
    glLineWidth( 1.0f );

    // --- TEKST ---
    glUseProgram( 0 );
    glDisable( GL_DEPTH_TEST );

    glMatrixMode( GL_PROJECTION ); glLoadMatrixf( &projection[ 0 ][ 0 ] );
    glMatrixMode( GL_MODELVIEW ); glLoadMatrixf( &view[ 0 ][ 0 ] );
    glColor3f( 0.0f, 0.0f, 0.0f );

    // 1. Wartości Y (przy PRZEDNIM słupku: backX, frontZ)
    for( int i = 0; i <= numTicks; i++ ) {
        float y = (h / numTicks) * i;
        float val = (maxVal / numTicks) * i;
        std::stringstream ss;
        ss << std::fixed << std::setprecision( 1 ) << val;

        // Offset
        float tx = backX + (dirBackX * 25.0f);
        if( dirBackX < 0 ) tx -= 50.0f; // Jeśli słupek z lewej, tekst bardziej w lewo

        // Rysujemy przy krawędzi frontZ, lekko wysunięte w stronę kamery (dirZ)
        drawString( tx, y, frontZ + (dirZ * 10.0f), ss.str() );
    }

    // Tytuł Y (nad osią Y z przodu)
    float titleY_X = backX + (dirBackX * 40.0f);
    if( dirBackX < 0 ) titleY_X -= 40.0f;
    // Podnosimy tytuł nad górną krawędź przedniego słupka
    drawString( titleY_X, h + 30.0f, frontZ, "Wartosc (Y)" );

    // 2. Tytuł Osi X (przy przedniej krawędzi frontZ)
    drawString( w / 2.0f - 50.0f, -50.0f, frontZ + (dirZ * 40.0f), "Osi Pomiary (X)" );

    // 3. Tytuł Osi Z (przy bocznej krawędzi frontX)
    float textXPos = frontX + (dirX * 50.0f);
    if( dirX < 0 ) textXPos -= 80.0f;
    drawString( textXPos, -50.0f, d / 2.0f, "Serie (Z)" );

    glEnable( GL_DEPTH_TEST );
}