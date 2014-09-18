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

GLfloat xCoach = 0.0f;
GLfloat yCoach = 1.0f;

GLfloat xrsize = 40;
GLfloat yrsize = 25;
GLfloat radius = 8.0f; // wheels radius


//Size of a step (speed)
GLfloat xBackgrStep = 1.0f;
GLfloat yBackgrStep = 1.0f;


float x2,y2 = 0.0f;
 
int j = 0;
//GLfloat tmpRadius = 0.0f;


void DrawCircle(int xPos, int yPos) {
	glColor3f(0.7f, 0.7f, 0.3f);

	glBegin(GL_TRIANGLE_FAN);
	
	for (j = 0; j < 360; j += 5)
	{
		glVertex2f(xPos + x2, yPos + y2);
		x2 = (float)radius * cos(j);
		y2 = (float)radius * sin(j);
		glVertex2f(xPos + x2, yPos + y2);
	}
	
	glEnd();
}

void DrawTrampoline(int xPos, int yPos)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-50.0 + xPos, 0.0 + yPos, 0.0);
	glVertex3f(0.0 + xPos, 30.0 + yPos, 0.0);
	glVertex3f(0.0 + xPos, 0.0 + yPos, 0.0);
	glEnd();
}

void DrawCoach(int xPos, int yPos)
{
	//coach body
	glColor3f(0.48f, 0.17f, 0.08f);	
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

	//windows
	glColor3f(0.8f, 0.7f, 0.5f);
	glRectf(xCoach , yCoach - 3, xCoach + 10, yCoach - 13); //left
	glRectf(xCoach + 30, yCoach - 3, xCoach + 40, yCoach - 13); //right
	
	//door
	glColor3f(0.4f, 0.3f, 0.2f);
	glRectf(xCoach + 14, yCoach + - 3, xCoach + 26, yCoach - 27);

	//Wheels
	DrawCircle(xCoach - 5, yCoach - 23); // left
	DrawCircle(xCoach + 45, yCoach - 23); //right
}

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
	{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.3f, 0.7f, 0.4f);
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
    // Reverse direction when you reach left or right edge
    /*if(xBackground > windowWidth-xrsize || xBackground < -windowWidth)
        xBackgrStep = -xBackgrStep;*/

    // Reverse direction when you reach top or bottom edge
    /*if(yBackground > windowHeight || yBackground < -windowHeight + xrsize)
        yBackgrStep = -yBackgrStep;*/
		xBackground -= xBackgrStep;
    //yBackground += yBackgrStep;

    // Check bounds. This is in case the window is made
    // smaller while the rectangle is bouncing and the 
	// rectangle suddenly finds itself outside the new
    // clipping volume
    /*if(xBackground > (windowWidth-rsize + xBackgrStep))
        xBackground = windowWidth-rsize-1;
	else if(xBackground < -(windowWidth + xBackgrStep))
		xBackground = -windowWidth -1;

    if(yBackground > (windowHeight + yBackgrStep))
        yBackground = windowHeight-1; 
	else if(yBackground < -(windowHeight - rsize + yBackgrStep))
		yBackground = -windowHeight + rsize - 1;*/
    glutPostRedisplay();
    glutTimerFunc(25,TimerFunction, 1);
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
	glutInitWindowSize(800,600);
        glutCreateWindow("Bounce");

	glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);

	SetupRC();

	glutMainLoop();
        
        return 0;
    }
