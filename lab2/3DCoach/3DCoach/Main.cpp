#include "OpenGLSB.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
float angle = 0.0f;
float lx = 0.0f, lz = -12.0f;
float x = -3.0f, z = 4.0f;
float deltaAngle = 0.0f;
float coachPos = 0.0f;
float coachSpan = 20.0f;
float deltaMove = 0;
float wheelAngle = 0;
float rotationCount = 0;
float rangeFromPlayer = -7;
float coachAngle = 0;
float trampolinePos = 7;
float coachJumpHeight = 0;
float flightLength = 11.0f;
bool isCrashed = false;
float wheel1Speed = 0;

PFNGLACTIVETEXTUREARBPROC __myglextActiveTextureARB;
PFNGLACTIVETEXTUREARBPROC __myglextActiveTexture;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2f = NULL;
#define glActiveTextureARB __myglextActiveTextureARB
#define glActiveTexture __myglextActiveTexture

void initGLextensions() {
	__myglextActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	__myglextActiveTexture = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTexture");
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)
		wglGetProcAddress("glMultiTexCoord2f");
}


VECTOR3D lightPosition(-0.1f, 58, 0);
GLuint shadowMapTexture;
const int shadowMapSize = 600;
MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;
MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
int windowWidth = 1200, windowHeight = 600;
int imageWidth, imageHeight;
float ratio = 2;
GLuint kover;
GLuint coachText;
GLuint bricks;
unsigned char* image;
bool isSeconRender = false;
bool pause = false;

std::vector< glm::vec3 > vertices;
std::vector< glm::vec3 > vertices_norm;
std::vector< glm::vec2 > vertices_text;
std::vector< glm::vec3 > wheel1;
std::vector< glm::vec3 > wheel1_norm;
std::vector< glm::vec3 > wheel2;
std::vector< glm::vec3 > wheel2_norm;
std::vector< glm::vec3 > tramploine;
std::vector< glm::vec3 > tramploine_norm;
std::vector< glm::vec2 > trampoline_text;
std::vector< glm::vec2 > wheel1_text;
std::vector< glm::vec2 > wheel2_text;
std::vector< glm::vec3 > cylinder;
std::vector< glm::vec3 > cylinder_norm;
std::vector< glm::vec2 > cylinder_text;

std::vector< glm::vec3 > loadObj(string path, std::vector< glm::vec3 > *result_norm_vertices,
	std::vector< glm::vec2 > *result_text_vertices)
{
	std::vector< unsigned int > vertexIndices;
	std::vector< unsigned int > UVIndices;
	std::vector< unsigned int > normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec3 > temp_normal_vertices;
	std::vector< glm::vec2 > temp_text_vertices;
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
			if (word == "vn") {
				glm::vec3 vertex;
				myfile >> vertex.x;
				myfile >> vertex.y;
				myfile >> vertex.z;
				temp_normal_vertices.push_back(vertex);
			}
			if (word == "vt") {
				glm::vec2 vertex;
				myfile >> vertex.x;
				myfile >> vertex.y;
				temp_text_vertices.push_back(vertex);
			}
			else if (word == "f"){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3];
				unsigned int UVIndex[3];
				unsigned int normalIndex[3];
				string dot;
				string normal;
				string rest;
				string UV;
				int pos = 0;
				std::string delimiter = "//";
				myfile >> word;
				pos = word.find(delimiter);
				if (pos != -1){
					dot = word.substr(0, pos);
					normal = word.substr(pos + delimiter.length(), word.length());
					vertexIndex[0] = atoll(dot.c_str());
					normalIndex[0] = atoll(normal.c_str());

					myfile >> word;
					pos = word.find(delimiter);
					dot = word.substr(0, pos);
					normal = word.substr(pos + delimiter.length(), word.length());
					vertexIndex[1] = atoll(dot.c_str());
					normalIndex[1] = atoll(normal.c_str());

					myfile >> word;
					pos = word.find(delimiter);
					dot = word.substr(0, pos);
					normal = word.substr(pos + delimiter.length(), word.length());
					vertexIndex[2] = atoll(dot.c_str());
					normalIndex[2] = atoll(normal.c_str());

					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
				else {
					delimiter = "/";
					pos = word.find(delimiter);

					dot = word.substr(0, pos);
					rest = word.substr(pos + delimiter.length(), word.length());
					pos = rest.find(delimiter);
					UV = rest.substr(0, pos);
					normal = rest.substr(pos + delimiter.length(), word.length());

					vertexIndex[0] = atoll(dot.c_str());
					UVIndex[0] = atoll(UV.c_str());
					normalIndex[0] = atoll(normal.c_str());

					myfile >> word;
					pos = word.find(delimiter);
					dot = word.substr(0, pos);
					rest = word.substr(pos + delimiter.length(), word.length());
					pos = rest.find(delimiter);
					UV = rest.substr(0, pos);
					normal = rest.substr(pos + delimiter.length(), word.length());

					vertexIndex[1] = atoll(dot.c_str());
					UVIndex[1] = atoll(UV.c_str());
					normalIndex[1] = atoll(normal.c_str());

					myfile >> word;
					pos = word.find(delimiter);
					dot = word.substr(0, pos);
					rest = word.substr(pos + delimiter.length(), word.length());
					pos = rest.find(delimiter);
					UV = rest.substr(0, pos);
					normal = rest.substr(pos + delimiter.length(), word.length());

					vertexIndex[2] = atoll(dot.c_str());
					UVIndex[2] = atoll(UV.c_str());
					normalIndex[2] = atoll(normal.c_str());

					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					UVIndices.push_back(UVIndex[0]);
					UVIndices.push_back(UVIndex[1]);
					UVIndices.push_back(UVIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);

				}
			}
		}
		myfile.close();
	}

	for (unsigned int i = 0; i < UVIndices.size(); i++){
		unsigned int UVIndex = UVIndices[i];
		glm::vec2 UV = temp_text_vertices[UVIndex - 1];
		result_text_vertices->push_back(UV);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++){
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normal_vertices[normalIndex - 1];
		result_norm_vertices->push_back(normal);
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
				glNormal3f(vertices_norm[i].x, vertices_norm[i].y, vertices_norm[i].z);
				glMultiTexCoord2f(GL_TEXTURE3, vertices_text[i].x, vertices_text[i].y);
				glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			}			
			glEnd();			
		glPopMatrix();	
	glPopMatrix();	
}

void DrawCylinder(){
	glPushMatrix();
	glTranslatef(0, 0, rangeFromPlayer + -1.5);
	glRotatef(0, 0, 0, 0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < cylinder.size(); i++){
		glNormal3f(cylinder_norm[i].x, cylinder_norm[i].y, cylinder_norm[i].z);
		//glMultiTexCoord2f(GL_TEXTURE1, trampoline_text[i].x, trampoline_text[i].y);
		glVertex3f(cylinder[i].x, cylinder[i].y, cylinder[i].z);
	}
	glEnd();
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
						glNormal3f(wheel1_norm[i].x, wheel1_norm[i].y, wheel1_norm[i].z);
						glTexCoord2f(wheel1_text[i].x, wheel1_text[i].y);
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
						glNormal3f(wheel1_norm[i].x, wheel1_norm[i].y, wheel1_norm[i].z);
						glTexCoord2f(wheel1_text[i].x, wheel1_text[i].y);
						glVertex3f(wheel1[i].x, wheel1[i].y, wheel1[i].z);
					}
				glEnd();
			glPopMatrix();		
		glPopMatrix();

		//small wheel 1
		glPushMatrix();		
		glTranslatef(-1.6f, 0.55f + wheel1Speed / 3.5, -2.6f + wheel1Speed * 1.5f);
			glPushMatrix(); 
				glRotatef(wheelAngle, 0, 0, 1);
				glBegin(GL_TRIANGLES);
					for (int i = 0; i < wheel2.size(); i++){
						glNormal3f(wheel2_norm[i].x, wheel2_norm[i].y, wheel2_norm[i].z);
						glTexCoord2f(wheel2_text[i].x, wheel2_text[i].y);
						glVertex3f(wheel2[i].x, wheel2[i].y, wheel2[i].z);
					}
				glEnd();
			glPopMatrix();		
		glPopMatrix();

		//small wheel 2
		glPushMatrix();		
		glTranslatef(-1.6f, 0.55f + wheel1Speed / 2, -0.5f - wheel1Speed);
			glPushMatrix(); 
				glRotatef(wheelAngle, 0, 0, 1);
				glBegin(GL_TRIANGLES);
					for (int i = 0; i < wheel2.size(); i++){
						glNormal3f(wheel2_norm[i].x, wheel2_norm[i].y, wheel2_norm[i].z);
						glTexCoord2f(wheel2_text[i].x, wheel2_text[i].y);
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
	glTranslatef(trampolinePos, 0, rangeFromPlayer + -1.5);
	glRotatef(180, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tramploine.size(); i++){
		glNormal3f(tramploine_norm[i].x, tramploine_norm[i].y, tramploine_norm[i].z);
		glMultiTexCoord2f(GL_TEXTURE1, trampoline_text[i].x, trampoline_text[i].y);
		glVertex3f(tramploine[i].x, tramploine[i].y, tramploine[i].z);
	}
	glEnd();
	glPopMatrix();
}

void DrawTrampolineLighted()
{
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(trampolinePos, 0, rangeFromPlayer + -1.5);
	glRotatef(180, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tramploine.size(); i++){
		glNormal3f(tramploine_norm[i].x, tramploine_norm[i].y, tramploine_norm[i].z);
		glMultiTexCoord2f(GL_TEXTURE1, trampoline_text[i].x, trampoline_text[i].y);
		glMultiTexCoord2f(GL_TEXTURE2, trampoline_text[i].x, trampoline_text[i].y);
		glVertex3f(tramploine[i].x, tramploine[i].y, tramploine[i].z);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
}

void DrawFloor(){	
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, -10.0f, -10.0f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, -10.0f, 10.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, 10.0f, 10.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, 10.0f, -10.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
}

void DrawFloorLighted(){
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, -10.0f, -10.0f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, -10.0f, 10.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, 10.0f, 10.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glNormal3f(0, 1, 0);
	glMultiTexCoord2f(GL_TEXTURE4, 10.0f, -10.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
}

void DrawScene(){	
	if (isSeconRender){	
		glActiveTexture(GL_TEXTURE4);
		glEnable(GL_TEXTURE_2D);
		DrawFloorLighted();
		glDisable(GL_TEXTURE_2D);
		DrawTrampolineLighted();	
		glActiveTexture(GL_TEXTURE2);
		return;
	}

	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	DrawFloor();
	glDisable(GL_TEXTURE_2D);
}

void RenderScene(void)
{	
	#pragma region first
	//First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);
	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_SMOOTH);
	glColorMask(0, 0, 0, 0);
	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);	
	//Draw the scene	
	glColor3f(1, 1, 1);
	DrawFloor();
	DrawCylinder();
	DrawCoach();
	DrawTrampoline();
	DrawWheels();
	//Read the depth buffer into the shadow map texture
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);
	glDisable(GL_TEXTURE_2D);
	//restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);
	#pragma endregion

	#pragma region second
	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);
	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, 15.0f, z,
		x - coachPos + coachSpan, 0.0f, z + lz,
		-0.0f, 1.0f, 0);

	glViewport(0, 0, windowWidth, windowHeight);
	//Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(lightPosition));
	glLightfv(GL_LIGHT1, GL_AMBIENT, white*0.2f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white*0.4f);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	DrawScene();
	glColor3f(0, 0, 0);
	DrawCylinder();
	DrawWheels();
	glColor3f(1, 1, 1);

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	DrawCoach();
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	DrawTrampoline();
	glDisable(GL_TEXTURE_2D);
	#pragma endregion
	
	#pragma region third
	//3rd pass
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);

	//Calculate texture matrix for projection
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);	
	MATRIX4X4 textureMatrix = biasMatrix*lightProjectionMatrix*lightViewMatrix;	
	
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);

	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);		
	
	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	isSeconRender = true;
	glColor3f(0.1f, 0.1f, 0.1f);
	DrawCylinder();
	glColor3f(1, 1, 1);
	DrawScene();		
	isSeconRender = false;

	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
#pragma endregion
}

void TimerFunc(int value)
{
	if (!pause){

		if (coachPos < coachSpan + 40){
			if (!isCrashed){
				coachPos += 0.3f;
			}
		}
		if (coachSpan - coachPos - 3 <= trampolinePos + 1.5 && coachAngle > -25
			&& coachSpan - coachPos >= trampolinePos - flightLength){
			coachAngle -= 1;
			coachPos += 0.09f;
			coachJumpHeight += 0.01;
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
			if (coachJumpHeight <= -0.8f){
				isCrashed = true;
			}
		}
		wheelAngle += 5;

		if (coachJumpHeight < -0.01) {
			wheel1Speed++;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(1, TimerFunc, 1);
}

void changeSize(int w, int h) {

}

void PressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.02f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.02f; break;
	case GLUT_KEY_UP: deltaMove = 0.9f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.9f; break;
	case GLUT_KEY_HOME: pause = true; break;
	case GLUT_KEY_END: pause = false; break;
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

void Init(void)
{
	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	

	glGenTextures(1, &bricks);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bricks);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
	glEnable(GL_TEXTURE_2D);
	image = SOIL_load_image("floor2.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(image);
	glDisable(GL_TEXTURE_2D);

	glGenTextures(1, &coachText);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, coachText);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
	glEnable(GL_TEXTURE_2D);
	image = SOIL_load_image("wood2.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(image);
	glDisable(GL_TEXTURE_2D);


	glGenTextures(1, &kover);		
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, kover);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
	glEnable(GL_TEXTURE_2D);
	image = SOIL_load_image("floor.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SOIL_free_image_data(image);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_CULL_FACE);	
	
	//glEnable(GL_TEXTURE_2D);
	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glDisable(GL_TEXTURE_2D);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, ratio, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	gluPerspective(45.0f, 1, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(lightPosition.x, lightPosition.y, lightPosition.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 60);
	glutInitWindowSize(1200, 600);
	glutCreateWindow("Coach adventures in 3D");

	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutTimerFunc(14, TimerFunc, 1);

	initGLextensions();

	vertices = loadObj("D:\\coachUV2.obj",  &vertices_norm, &vertices_text);
	wheel1 = loadObj("D:\\newWheel3.obj", &wheel1_norm, &wheel1_text);

	wheel2 = loadObj("D:\\newWheel2.obj", &wheel2_norm, &wheel2_text);
	tramploine = loadObj("D:\\trampUV3.obj", &tramploine_norm, &trampoline_text);

	cylinder = loadObj("D:\\cylinder.obj", &cylinder_norm, &cylinder_text);

	glutSpecialFunc(PressKey);
	glutSpecialUpFunc(ReleaseKey);

	glutReshapeFunc(changeSize);
	Init();
	
	glutMainLoop();

	return 0;
}