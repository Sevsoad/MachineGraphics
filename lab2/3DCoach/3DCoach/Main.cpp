#include "OpenGLSB.h"	
#include <vector>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "glm\glm.hpp"

using namespace std;

// angle of rotation for the camera direction
float angle = 0.0f;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 7.0f;

float deltaAngle = 0.0f;
float coachPos = 0.0f;
float coachSpan = 40.0f;
float deltaMove = 0;
float wheelAngle = 0;
float rotationCount = 0;
float rangeFromPlayer = -7;
float coachAngle = 0;
float trampolinePos = 7;
float coachJumpHeight = 0;
float flightLength = 14.0f;
bool isCrashed = false;
float wheel1Speed = 0;
std::vector< glm::vec3 > vertices;
std::vector< glm::vec3 > wheel1;
std::vector< glm::vec3 > wheel2;
std::vector< glm::vec3 > tramploine;

std::vector< glm::vec3 > loadObj(string path)
{
	std::vector< unsigned int > vertexIndices;//, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec3 > result_vertices;
	string word;

	ifstream myfile(path);
	if (myfile.is_open())
	{
		while (myfile >> word)
		{
			if (word == "v"){
				glm::vec3 vertex;
				myfile >> vertex.x;
				myfile >> vertex.y;
				myfile >> vertex.z;
				temp_vertices.push_back(vertex);
			}
			
			else if (word == "f"){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3];
				std::string delimiter = "//";
				myfile >> word;
				vertexIndex[0] = atoll(word.c_str());
				myfile >> word;
				vertexIndex[1] = atoll(word.c_str());
				myfile >> word;
				vertexIndex[2] = atoll(word.c_str());

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);				
			}
		}
		myfile.close();
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		result_vertices.push_back(vertex);
	}

	return result_vertices;
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void DrawCoach() {
	glPushMatrix();
	glTranslatef(coachSpan - coachPos, 0.85f + coachJumpHeight, -1.5f + rangeFromPlayer);
		glPushMatrix();
			glRotatef(coachAngle, 0, 0, 1);
			glBegin(GL_TRIANGLES);	
			for (int i = 0; i < vertices.size(); i++){
				glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			}
			glEnd();
		glPopMatrix();	
	glPopMatrix();	
}

void DrawWheels()
{
	glPushMatrix();
		glTranslatef(coachSpan - coachPos, coachJumpHeight, rangeFromPlayer);
		glRotatef(coachAngle, 0, 0, 1);
		//big wheel 1
		glPushMatrix();		
		glTranslatef(2.0 + wheel1Speed / 1.5, 0.89f + wheel1Speed/ 4, -0.55f + wheel1Speed);
			glPushMatrix(); 
				glRotatef(wheelAngle, 0, 0, 1);
				glBegin(GL_TRIANGLES);
					for (int i = 0; i < wheel1.size(); i++){
						glVertex3f(wheel1[i].x, wheel1[i].y, wheel1[i].z);
					}
				glEnd();
			glPopMatrix();		
		glPopMatrix();

		//big wheel2
		glPushMatrix();		
		glTranslatef(2.0, 0.89f + wheel1Speed / 1.5, -2.5f - wheel1Speed);
			glPushMatrix(); 
				glRotatef(wheelAngle, 0, 0, 1);
				glBegin(GL_TRIANGLES);
					for (int i = 0; i < wheel1.size(); i++){
						glVertex3f(wheel1[i].x, wheel1[i].y, wheel1[i].z);
					}
				glEnd();
			glPopMatrix();		
		glPopMatrix();

		//small wheel 1
		glPushMatrix();		
		glTranslatef(-1.5f + wheel1Speed/ 1.8, 0.65f + wheel1Speed, -2.6f + wheel1Speed);
			glPushMatrix(); 
				glRotatef(wheelAngle, 0, 0, 1);
				glBegin(GL_TRIANGLES);
					for (int i = 0; i < wheel2.size(); i++){
						glVertex3f(wheel2[i].x, wheel2[i].y, wheel2[i].z);
					}
				glEnd();
			glPopMatrix();		
		glPopMatrix();

		//small wheel 2
		glPushMatrix();		
		glTranslatef(-1.5f, 0.65f + wheel1Speed / 3, -0.5f - wheel1Speed/3);
			glPushMatrix(); 
				glRotatef(wheelAngle, 0, 0, 1);
				glBegin(GL_TRIANGLES);
					for (int i = 0; i < wheel2.size(); i++){
						glVertex3f(wheel2[i].x, wheel2[i].y, wheel2[i].z);
					}
				glEnd();
			glPopMatrix();		
		glPopMatrix();

	glPopMatrix();
}

void DrawTrampoline()
{
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.3f);
	glTranslatef(trampolinePos, 0, rangeFromPlayer + -1.5);
	glRotatef(-90, 0, 1, 0);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tramploine.size(); i++){
		glVertex3f(tramploine[i].x, tramploine[i].y, tramploine[i].z);
	}
	glEnd();
	glPopMatrix();
}

void RenderScene(void)
{
	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		-0.0f, 1.0f, 0.0f);

	GLfloat color[] = { 0.384f, 0.776f, 0.533f, 1.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	GLfloat color2[] = { 1.f, 0.8f, 1.f, 0.2f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color2);
	DrawCoach();

	GLfloat color3[] = { 0.862, 0.803, 0.156, 0.2f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color3);
	DrawWheels();

	GLfloat color4[] = { 0.6, 0.301, 0.058, 0.2f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color4);
	DrawTrampoline();

	glutSwapBuffers();
}

void TimerFunc(int value)
{
	if (coachPos < coachSpan + 40){
		if (!isCrashed){
			coachPos += 0.3f;
		}
	}
	if (coachSpan - coachPos - 3 <= trampolinePos && coachAngle > -25
		&& coachSpan - coachPos >= trampolinePos - flightLength){
		coachAngle -= 1;
		coachPos += 0.09f;
	}
	if (coachSpan - coachPos - 3 <= trampolinePos
		&& coachSpan - coachPos >= trampolinePos - flightLength){
		coachJumpHeight += 0.1;
	}
	if (coachSpan - coachPos - 3 < trampolinePos - flightLength
		&& coachJumpHeight > -0.8){
		if (coachAngle <= 0){
			coachAngle += 0.5;
		}
		coachJumpHeight -= 0.1;
		if (coachJumpHeight <= -0.8){
			isCrashed = true;
		}
	}
	wheelAngle += 5;

	if (coachJumpHeight <= -0.3) {
		wheel1Speed++;
	}

	glutPostRedisplay();
	glutTimerFunc(14, TimerFunc, 1);
}

void changeSize(int w, int h) {

	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 50.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void PressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.02f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.02f; break;
	case GLUT_KEY_UP: deltaMove = 0.9f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.9f; break;
	}
}

void ReleaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = 0.0f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
	case GLUT_KEY_UP: deltaMove = 0; break;
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void SetupRC(void)
{
	glClearColor(0.2f, 0.2f, 0.5f, 1.0f);	
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 60);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("Coach adventures in 3D");

	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutTimerFunc(14, TimerFunc, 1);

	vertices = loadObj("D:\\coach5.obj");
	wheel1 = loadObj("D:\\wheel4.obj");
	wheel2 = loadObj("D:\\wheel3.obj");
	tramploine = loadObj("D:\\tramploine.obj");

	glutSpecialFunc(PressKey);
	glutSpecialUpFunc(ReleaseKey);

	glutReshapeFunc(changeSize);
	SetupRC();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = { -7, 0.0, 10, 0.2};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	glutMainLoop();

	return 0;
}