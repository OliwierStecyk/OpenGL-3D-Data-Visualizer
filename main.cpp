#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include "tekstura.h"

//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <vector>

// Wczytywanie plików 

#include "DataLoader.h"
DataLoader daneProjektu;



//

//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX = 10;
double kameraZ = 20;
double kameraD = -800;
double kameraPredkosc;
double kameraKat = -20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;

//macierze

glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy
glm::mat4 MVP;
glm::vec3 lightPos(100, 100.0f,500.0f);

GLuint lightColor_id = 0;
GLuint lightPos_id = 0;
GLuint viewPos_id = 0;
GLuint alfa_id = 0;
GLuint  MVP_id = 0;




GLuint materialambient_id = 0;
GLuint materialdiffuse_id = 0;
GLuint materialspecular_id = 0;
GLfloat materialshine_id = 0;


//shaders
GLuint programID = 0;




unsigned int VBO, sphereVAO,  ebo;
using namespace std;
// Sphere vertex data
std::vector<float> vertices;
// Indices for the sphere mesh
std::vector<unsigned int> indices;


/*###############################################################*/
void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount) {
	float x, y, z, xy;                               // Vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;     // Normal vectors
	float s, t;                                      // Texture coordinates
	unsigned int idx;

	// Generate the sphere vertices
	for (unsigned int i = 0; i <= stackCount; ++i) {
		float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() /
			stackCount;
		xy = radius * cosf(stackAngle);  // Radius at current stack
		z = radius * sinf(stackAngle);   // z-coordinate

		// Add vertices
		for (unsigned int j = 0; j <= sectorCount; ++j) {
			float sectorAngle = j * 2 * glm::pi<float>() / sectorCount;
			x = xy * cosf(sectorAngle);    // X position
			y = xy * sinf(sectorAngle);    // Y position

			// Normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			// Texture coordinates (s, t)
			s = (float)j / sectorCount;
			t = (float)i / stackCount;

			// Add to the vertex buffer
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(nx);
			vertices.push_back(ny);
			vertices.push_back(nz);
			vertices.push_back(s);
			vertices.push_back(t);
		}
	}
	// Indices for the sphere mesh

	for (unsigned int i = 0; i < stackCount; ++i) {
		unsigned int k1 = i * (sectorCount + 1);
		unsigned int k2 = k1 + sectorCount + 1;

		for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			if (i != 0) {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stackCount - 1)) {
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}
/*###############################################################*/
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
	}

}
/******************************************/


void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case '1':
		lightPos[1] += 10;
		break;
	case '2':
		lightPos[1] += -10;
		break;

	}
	
	
}
/*###############################################################*/
void rysuj(void)
{
	


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);

	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(0, -200, kameraD -200));
	MV = glm::rotate(MV, (float)glm::radians(kameraZ ), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX ), glm::vec3(0, 1, 0));
	
    // kula

	glUseProgram(programID);

	MVP_id = glGetUniformLocation(programID, "MVP");

	MV = glm::translate(MV, glm::vec3(0, 300, 0));
	MVP = P * MV;
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
	glUniform3f(lightColor_id, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPos_id, lightPos[0], lightPos[1], lightPos[2]);

	///material1
	glUniform3f(materialambient_id, 0.19125f, 0.0735f, 0.0225f);
	glUniform3f(materialdiffuse_id, 0.7038f, 0.27048f, 0.0828f);
	glUniform3f(materialspecular_id, 0.256777f, 0.137622f, 0.086014f);
	glUniform1f(materialshine_id, 12.8f);
	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);



	//glFlush();
	glutSwapBuffers();

}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 2000.0f);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/



void timer(int value) {

	

	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 8");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
	glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	//glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)

	
	glEnable(GL_DEPTH_TEST);
		
	// Wczytywanie plików

	if( daneProjektu.load( "dane.txt" ) ) {
		printf( "Wczytano %d zestawow danych.\n", ( int ) daneProjektu.allData.size() );
	}

	//

	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	glUseProgram(programID);
	

	lightColor_id = glGetUniformLocation(programID, "lightColor");
    lightPos_id = glGetUniformLocation(programID, "lightPos");
	

	materialambient_id = glGetUniformLocation(programID, "material.ambient");
	materialdiffuse_id = glGetUniformLocation(programID, "material.diffuse");
	materialspecular_id = glGetUniformLocation(programID, "material.specular");
	materialshine_id = glGetUniformLocation(programID, "material.shininess");
	alfa_id = glGetUniformLocation(programID, "alfa");

	
	generateSphere(250.0f, 50, 50);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);



	glGenVertexArrays(1, &sphereVAO);

	glBindVertexArray(sphereVAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // Texture coords
	glEnableVertexAttribArray(2);
	
	

	glutMainLoop();				

	glDeleteVertexArrays(1, &sphereVAO);

	glDeleteBuffers(1, &VBO);


	return(0);

}

