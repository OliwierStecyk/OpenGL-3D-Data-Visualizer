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

// Funkcja pomocnicza do rysowania tekstu 2D (dla legendy)
void drawString2D( float x, float y, const std::string& text ) {
    glRasterPos2f( x, y );
    for( char c : text ) {
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, c );
    }
}

void SceneManager::drawGrid( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, int step, float backX, float backZ ) {
    glm::mat4 model = glm::mat4( 1.0f );
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    glLineWidth( 1.0f );
    glBegin( GL_LINES );
    glColor3f( 0.85f, 0.85f, 0.85f );

    for( float i = 0; i <= w; i += step ) { glVertex3f( i, 0, 0 ); glVertex3f( i, 0, d ); }
    for( float i = 0; i <= d; i += step ) { glVertex3f( 0, 0, i ); glVertex3f( w, 0, i ); }
    for( float i = 0; i <= d; i += step ) { glVertex3f( backX, 0, i ); glVertex3f( backX, h, i ); }
    for( float i = 0; i <= h; i += step ) { glVertex3f( backX, i, 0 ); glVertex3f( backX, i, d ); }
    for( float i = 0; i <= w; i += step ) { glVertex3f( i, 0, backZ ); glVertex3f( i, h, backZ ); }
    for( float i = 0; i <= h; i += step ) { glVertex3f( 0, i, backZ ); glVertex3f( w, i, backZ ); }
    glEnd();
}

void SceneManager::drawAxes( GLuint mvpID, glm::mat4 projection, glm::mat4 view, float w, float h, float d, float maxVal,
    float backX, float backZ, float frontX, float frontZ ) {
    glm::mat4 mvp = projection * view;
    glUniformMatrix4fv( mvpID, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

    float dirX = (frontX > 0.1f) ? 1.0f : -1.0f;
    float dirZ = (frontZ > 0.1f) ? 1.0f : -1.0f;
    float dirBackX = (backX > 0.1f) ? 1.0f : -1.0f;

    glLineWidth( 2.0f );
    glBegin( GL_LINES );
    glColor3f( 0.0f, 0.0f, 0.0f );

    // Ramka podłogi
    glVertex3f( 0, 0, 0 ); glVertex3f( w, 0, 0 );
    glVertex3f( w, 0, 0 ); glVertex3f( w, 0, d );
    glVertex3f( w, 0, d ); glVertex3f( 0, 0, d );
    glVertex3f( 0, 0, d ); glVertex3f( 0, 0, 0 );

    // Słupki pionowe
    glVertex3f( backX, 0, backZ ); glVertex3f( backX, h, backZ );
    glVertex3f( backX, 0, frontZ ); glVertex3f( backX, h, frontZ );

    // Górne krawędzie
    glVertex3f( backX, h, 0 ); glVertex3f( backX, h, d );
    glVertex3f( 0, h, backZ ); glVertex3f( w, h, backZ );

    // Podziałki (Ticks)
    int numTicks = 5;
    float tickLen = 15.0f;

    for( int i = 0; i <= numTicks; i++ ) {
        float y = (h / numTicks) * i;
        glVertex3f( backX, y, frontZ );
        glVertex3f( backX + (tickLen * dirBackX), y, frontZ );
    }
    for( int i = 0; i <= numTicks; i++ ) {
        float x = (w / numTicks) * i;
        glVertex3f( x, 0, frontZ );
        glVertex3f( x, 0, frontZ + (tickLen * dirZ) );
    }
    for( int i = 0; i <= numTicks; i++ ) {
        float z = (d / numTicks) * i;
        glVertex3f( frontX, 0, z );
        glVertex3f( frontX + (tickLen * dirX), 0, z );
    }
    glEnd();
    glLineWidth( 1.0f );

    // Teksty
    glUseProgram( 0 );
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION ); glLoadMatrixf( &projection[ 0 ][ 0 ] );
    glMatrixMode( GL_MODELVIEW ); glLoadMatrixf( &view[ 0 ][ 0 ] );
    glColor3f( 0.0f, 0.0f, 0.0f );

    // Wartości Y
    for( int i = 0; i <= numTicks; i++ ) {
        float y = (h / numTicks) * i;
        float val = (maxVal / numTicks) * i;
        std::stringstream ss;
        ss << std::fixed << std::setprecision( 1 ) << val;

        float tx = backX + (dirBackX * 25.0f);
        if( dirBackX < 0 ) tx -= 50.0f;
        drawString( tx, y, frontZ + (dirZ * 10.0f), ss.str() );
    }

    // Tytuł Y
    float titleY_X = backX + (dirBackX * 40.0f);
    if( dirBackX < 0 ) titleY_X -= 40.0f;
    drawString( titleY_X, h + 30.0f, frontZ, "Wartosc (Y)" );

    // Tytuł X
    drawString( w / 2.0f - 50.0f, -50.0f, frontZ + (dirZ * 40.0f), "Osi Pomiary (X)" );

    // USUNIĘTO: drawString(..., "Serie (Z)"); -> Rysujemy to teraz tylko w main.cpp

    glEnable( GL_DEPTH_TEST );
}

void SceneManager::drawLegend( int screenW, int screenH, const std::vector<std::string>& labels, const std::vector<glm::vec3>& colors ) {
    glUseProgram( 0 );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    glMatrixMode( GL_PROJECTION ); glPushMatrix(); glLoadIdentity();
    gluOrtho2D( 0, screenW, 0, screenH );
    glMatrixMode( GL_MODELVIEW ); glPushMatrix(); glLoadIdentity();

    float margin = 10.0f;
    float itemHeight = 20.0f;
    float titleHeight = 25.0f;
    float boxWidth = 150.0f;
    float boxHeight = labels.size() * itemHeight + 2 * margin + titleHeight;

    float startX = screenW - boxWidth - 20.0f;
    float startY = screenH - boxHeight - 20.0f;

    // Tło
    glColor3f( 1.0f, 1.0f, 1.0f );
    glBegin( GL_QUADS );
    glVertex2f( startX, startY );
    glVertex2f( startX + boxWidth, startY );
    glVertex2f( startX + boxWidth, startY + boxHeight );
    glVertex2f( startX, startY + boxHeight );
    glEnd();

    // Ramka
    glColor3f( 0.0f, 0.0f, 0.0f );
    glLineWidth( 1.0f );
    glBegin( GL_LINE_LOOP );
    glVertex2f( startX, startY );
    glVertex2f( startX + boxWidth, startY );
    glVertex2f( startX + boxWidth, startY + boxHeight );
    glVertex2f( startX, startY + boxHeight );
    glEnd();

    // Tytuł
    float textWidth = 7 * 9.0f;
    float titleX = startX + (boxWidth - textWidth) / 2.0f;
    float titleY = startY + boxHeight - margin - 10.0f;
    glColor3f( 0.0f, 0.0f, 0.0f );
    drawString2D( titleX, titleY, "LEGENDA" );

    // Elementy
    float currentY = titleY - itemHeight - 5.0f;
    for( size_t i = 0; i < labels.size(); i++ ) {
        int colorIdx = i % colors.size();
        glColor3f( colors[ colorIdx ].r, colors[ colorIdx ].g, colors[ colorIdx ].b );
        float rectX = startX + margin;

        glBegin( GL_QUADS );
        glVertex2f( rectX, currentY );
        glVertex2f( rectX + 10.0f, currentY );
        glVertex2f( rectX + 10.0f, currentY + 10.0f );
        glVertex2f( rectX, currentY + 10.0f );
        glEnd();

        glColor3f( 0.0f, 0.0f, 0.0f );
        glBegin( GL_LINE_LOOP );
        glVertex2f( rectX, currentY );
        glVertex2f( rectX + 10.0f, currentY );
        glVertex2f( rectX + 10.0f, currentY + 10.0f );
        glVertex2f( rectX, currentY + 10.0f );
        glEnd();

        drawString2D( rectX + 20.0f, currentY + 2.0f, labels[ i ] );
        currentY -= itemHeight;
    }

    glMatrixMode( GL_PROJECTION ); glPopMatrix();
    glMatrixMode( GL_MODELVIEW ); glPopMatrix();
    glEnable( GL_DEPTH_TEST );
}