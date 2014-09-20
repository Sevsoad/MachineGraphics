#include "OpenGLSB.h"	
#include <stdio.h>
#include <math.h>
#define PI 3.14159265358979323846


// Keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

// Object coordinates on screen
GLfloat xBackground = 150.0f;
GLfloat yBackground = -30.0f;

GLfloat xCoach = -150.0f;
GLfloat yCoach = 1.0f;

GLfloat wheelRadius = 8.0f; 
int spokesRotationDelay = 20;
int spokesRotationTime = 0;


//Size of a step (speed)
GLfloat xBackgrStep = 2.0f;
GLfloat yBackgrStep = 2.0f;

GLfloat xCoachStep = 2.0f;
float x2,y2 = 0.0f; //tmp values to draw circle
 
int j = 0;


void DrawCircle(int xPos, int yPos) {
	glColor3ub(139, 119, 101);

	glBegin(GL_TRIANGLE_FAN);
	
	for (j = 0; j < 360; j += 5)
	{
		x2 = (float)wheelRadius * cos(j);
		y2 = (float)wheelRadius * sin(j);
		glVertex2f(xPos + x2, yPos + y2);
	}
	
	glEnd();
}

void DrawSpokes(GLfloat xPos, GLfloat yPos)
{
	glColor3ub(255, 127, 36);
	glBegin(GL_LINES);
	if (spokesRotationTime >= spokesRotationDelay)
	{
		
		glVertex2f(xPos - wheelRadius/1.4f, yPos + wheelRadius/1.5f);
		glVertex2f(xPos + wheelRadius / 1.4f, yPos - wheelRadius / 1.5f);
		glVertex2f(xPos + wheelRadius / 1.4f, yPos + wheelRadius / 1.5f);
		glVertex2f(xPos - wheelRadius / 1.4f, yPos - wheelRadius / 1.5f);
		
		spokesRotationTime++;
		if (spokesRotationTime >= spokesRotationDelay * 2)
		{
			spokesRotationTime = 0;
		}
	}
	else 
	{
		glVertex2f(xPos - wheelRadius, yPos );
		glVertex2f(xPos + wheelRadius, yPos);
		glVertex2f(xPos, yPos - wheelRadius);
		glVertex2f(xPos, yPos + wheelRadius);
		spokesRotationTime++;
	}
	glEnd();	
}

void DrawTrampoline(int xWheel, int yWheel)
{
	glColor3ub(135, 135, 135);
	glBegin(GL_TRIANGLES);
	glVertex3f(-50.0 + xWheel, 0.0 + yWheel, 0.0);
	glVertex3f(0.0 + xWheel, 30.0 + yWheel, 0.0);
	glVertex3f(0.0 + xWheel, 0.0 + yWheel, 0.0);
	glEnd();
}

void DrawCoach()
{
	//coach body
	glColor3ub(77, 77, 77);
	glRectf(xCoach + 10, yCoach, xCoach +  30, yCoach - 28);
	glRectf(xCoach - 5, yCoach, xCoach + 10, yCoach - 18);
	glRectf(xCoach, yCoach, xCoach + 45, yCoach - 18);

	glBegin(GL_TRIANGLES);
	glVertex3f(xCoach - 5, yCoach, 0.0);
	glVertex3f(xCoach - 9, yCoach, 0.0);
	glVertex3f(xCoach - 5, yCoach - 18, 0.0);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(xCoach + 45, yCoach, 0.0);
	glVertex3f(xCoach + 49, yCoach, 0.0);
	glVertex3f(xCoach + 45, yCoach - 18, 0.0);
	glEnd();

	//roof
	glColor3ub(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xCoach - 9, yCoach);
	glVertex2f(xCoach , yCoach + 8);
	glVertex2f(xCoach + 5, yCoach + 10);
	glVertex2f(xCoach + 36, yCoach + 10);
	glVertex2f(xCoach + 41, yCoach + 8);
	glVertex2f(xCoach + 49, yCoach);
	glEnd();

	//windows
	glColor3ub(244, 164, 96);
	glRectf(xCoach + 3, yCoach - 3, xCoach + 9, yCoach - 13); //left
	glRectf(xCoach + 31, yCoach - 3, xCoach + 37, yCoach - 13); //right
	
	//door
	glColor3ub(94, 94, 94);
	glRectf(xCoach + 14, yCoach + - 3, xCoach + 26, yCoach - 27);

	//Wheels
	DrawCircle(xCoach - 5, yCoach - 23); // left
	DrawCircle(xCoach + 45, yCoach - 23); //right

	//Rotating spokes
	DrawSpokes(xCoach - 5, yCoach - 23);
	DrawSpokes(xCoach + 45, yCoach - 23);
}


///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
	{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(0, 100, 0);
	glRectf(-400, -30, 400, -300);

	DrawTrampoline(xBackground, yBackground);

	DrawCoach(0, 0);

    // Flush drawing commands and swap
    glutSwapBuffers();
	}

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
{
	xBackground -= xBackgrStep;
	xCoach += xCoachStep;

	glutPostRedisplay();
	glutTimerFunc(25, TimerFunction, 1);
}


///////////////////////////////////////////////////////////
// Setup the rendering state
void SetupRC(void)
    {
    // Set clear color to blue
    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    }


///////////////////////////////////////////////////////////
// Called by GLUT library when the window has chanaged size
void ChangeSize(int w, int h)
    {
    GLfloat aspectRatio;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;
		
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h) 
        {
        windowWidth = 100;
        windowHeight = 100 / aspectRatio;
        glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
        }
    else 
        {
        windowWidth = 100 * aspectRatio;
        windowHeight = 100;
        glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
        }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }

///////////////////////////////////////////////////////////
// Main program entry point
int main(int argc, char* argv[])
	{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1200,600);
        glutCreateWindow("Bounce");

	glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);

	SetupRC();

	glutMainLoop();
        
        return 0;
    }
