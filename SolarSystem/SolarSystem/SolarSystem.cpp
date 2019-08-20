// SolarSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL\glut.h>
#include "windows.h"
#include <math.h>
#include <ctime>

#define sunRadius 0.4
#define earthRadius 0.06
#define moonRadius 0.016
void drawSolarSystem(void);
void Initialization(void);
void reshapeFunc(int x, int y);
void keyboard(unsigned char Key, int x, int y);
void displayFunc(void);
void idleFunc(void);
void reset();
void mouse(int btn, int state, int x, int y);
void setView();

GLfloat sunColor[4][3] = {{ 1.0, 0.8, 0.3 }, { 0.858824, 0.858824, 0.439216}, {1, .25, 0}, { 0.737255, 0.560784, 0.560784}};
GLfloat earthColor[4][3] = {{ 0.4, 0.6, 0.3 },{ 0.647059, 0.164706, 0.164706},{ 1, 0.498039, 0},{ 0.22, 0.69, 0.87}};
GLfloat moonColor[4][3] = {{ 0.3, 0.3, 0.5 },{ 0.752941, 0.752941, 0.752941},{ .85, .85, .95},{ 1, .11, .68}};
GLint colorS = 0, colorE = 0, colorM = 0;
GLfloat rotationSpeed = 0.05;
GLfloat dayInYear = 365;
GLfloat year = 0.0;
GLfloat day = 0.0;
GLfloat moonAroundEarth = 0.0;
GLfloat moonItsself = 0.0;
GLfloat earthOrbitRadius = 1.0;
GLfloat moonOrbitRadius = 0.1;
GLfloat moonAroundEarthRate = 2 * rotationSpeed;
GLfloat moonRotationItsselfRate = 3 * rotationSpeed;
GLfloat dayRate = 5 * rotationSpeed;
GLfloat yearRate = dayInYear / 360.0 * dayRate * rotationSpeed;
GLfloat eyeX = 0.0;
GLfloat eyeY = 0.0;
GLfloat eyeZ = -4.0;
GLfloat targetX = 0.0;
GLfloat targetY = 0.0;
GLfloat targetZ = 1.0;
GLfloat upX = 0.0;
GLfloat upY = -1.0;
GLfloat upZ = 0.0;
int spin_x = 0, spin_y = 0, old_x = 0, old_y = 0, look = 0;
double pi = 3.1415926535897;
bool earth = false, moon = false, sun = false, move = false;

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Solar System");
	Initialization();
	glutReshapeFunc(reshapeFunc);
	glutDisplayFunc(displayFunc);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idleFunc);
	glutMainLoop();
	return 0;
}

void drawSolarSystem() {
	//sets the location to the location of the moon
	if (sun) {
		eyeZ = 0.0;
	}
	//sets the location to the location of the earth
	if (earth) {
		eyeX = (earthOrbitRadius)*cos((year*pi/180)); 
		eyeZ = -(earthOrbitRadius)*sin((year*pi / 180));
		moonOrbitRadius = .5;
	}

	//sets the camera to the location of the moon
	if (moon) {
		eyeX = (earthOrbitRadius + moonOrbitRadius)*cos((year*pi / 180)) + (moonOrbitRadius)*cos(((moonAroundEarth)*pi / 180));
		eyeZ = -(earthOrbitRadius)*sin((year*pi / 180)) - (moonOrbitRadius)*sin(((moonAroundEarth)*pi / 180));
		moonOrbitRadius = .5;
	}

	if (!sun && !earth && !moon) { 
		setView();
	}

	glPushMatrix();
	gluLookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ);
	glColor3fv(sunColor[colorS]);
	glutSolidSphere(sunRadius, 50, 50);
	glPushMatrix();

	glRotatef(year, 0.0, 1.0, 0.0);
	glTranslatef(earthOrbitRadius, 0.0, 0.0);
	glRotatef(-year, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(day, 0.25, 1.0, 0.0);
	glColor3fv(earthColor[colorE]);
	glutSolidSphere(earthRadius, 10, 10);
	glPopMatrix();
	glRotatef(moonAroundEarth, 0.0, 1.0, 0.0);
	glTranslatef(moonOrbitRadius, 0.0, 0.0);
	glRotatef(-moonAroundEarth, 0.0, 1.0, 0.0);
	glRotatef(moonItsself, 0.0, 1.0, 0.0);

	glColor3fv(moonColor[colorM]);
	glutSolidSphere(moonRadius, 8, 8);
	glPopMatrix();
	glPopMatrix();
}


void Initialization(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(10.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshapeFunc(int x, int y) {
	if (y == 0 || x == 0) return;

	glLoadIdentity();
	gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);
	displayFunc();
}

void displayFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawSolarSystem();
	glFlush();
	glutSwapBuffers();
}

void idleFunc(void) {
	day += dayRate;
	year += yearRate;
	moonItsself += moonRotationItsselfRate;
	moonAroundEarth += moonAroundEarthRate;
	displayFunc();
}

void reset() {
	earth = false;
	moon = false;
	sun = false;
	eyeX = 0.0;
	eyeY = 0.0;
	eyeZ = -4.0;
	targetX = 0.0;
	targetY = 0.0;
	targetZ = 1.0;
	upX = 0.0;
	upY = -1.0;
	upZ = 0.0;
	moonOrbitRadius = .1;
}

void setView() {
	switch (look) {
	case 0:
		eyeX = 0.0;
		eyeY = 0.0;
		eyeZ = -4.0;
		targetX = 0.0;
		targetY = 0.0;
		targetZ = 1.0;
		upX = 0.0;
		upY = -1.0;
		upZ = 0.0;
		break;
	case 1:
		eyeX = 5.0;
		eyeY = 7.0;
		eyeZ = 0.0;
		targetX = 0.0;
		targetY = 0.0;
		targetZ = 0.0;
		upX = 0.0;
		upY = -1.0;
		upZ = 0.0;
		break;
	case 2:
		eyeX = 5;
		eyeY = -3.0;
		eyeZ = -4.0;
		targetX = 0.0;
		targetY = 0.0;
		targetZ = 0.0;
		upX = 0.0;
		upY = -1.0;
		upZ = 0.0;
		break;
	}
}

void keyboard(unsigned char Key, int x, int y) {
	switch (Key) {
	case 's':
	case 'S':
		reset();
		sun = true;
		break;
	case 'e':
	case 'E':
		reset();
		earth = true;
		break;
	case 'm':
	case 'M':
		reset();
		moon = true;
		break;
	case 'r':
	case 'R':
		reset();
		break;

	case '1':
		colorS++;
		
		if (colorS >= 4) {
			colorS = 0;
		}
		break;

	case '2':
		colorE++;

		if (colorE >= 4) {
			colorE = 0;
		}
		break;

	case '3':
		colorM++;

		if (colorM >= 4) {
			colorM = 0;
		}
		break;

	case ' ':
		printf("x; %f, y: %f, z:%f\n", eyeX, eyeY, eyeZ);
		printf("x; %f, y: %f, z:%f\n", targetX, targetY, targetZ);
		printf("x; %f, y: %f, z:%f\n", upX, upY, upZ);
		break;
	case 'Q':
	case 'q':
	case  27:   // ESC
		exit(0);
		break;
	}
}

void mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		look++;
		if (look == 3) {
			look = 0;
		}
		reset();

	}

	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
	}

	if (btn == 3) {
		eyeZ += .5;
	}

	if (btn == 4) {
		eyeZ -= .5;
	}
}

