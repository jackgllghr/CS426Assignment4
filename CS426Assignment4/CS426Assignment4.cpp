//CS426 Assignment 4, Jack Gallagher 69537058
//Press u or d to alter the velocity of the pendulum, and l or r to alter the angle
#include "stdafx.h"
#include <freeglut.h>
#include <math.h>

#define NUM 50000
#define FRAMETIMEMS 5  // One frame every five milliseconds
#define PI 3.142

void circle(float, float, float);
void draw_bob(float);
void gl_pnum(float, float, float);
void gl_pstring(float, float, char *);

// Declare variables for pendulum globally after main() 
float x[NUM], y[NUM];
float g = 9.81, l = 0.25, D=-0.5, dt = 0.0002;
float v = 15.1, a = 0, t = 0, b = 0, c = 0.0;
int i = 0;

int window1 = 0;

//Draw the pendulum
void draw_bob(float theta){
	float offset = PI / 2.0f;
	glColor3f(1.0, 0, 0);

	// Draw line from center to bob 
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(0.5f + (0.3f*cos(theta-offset)), 0.5f + (0.3f*sin(theta-offset)));
	glEnd();
	circle(0.5f + (0.3f*cos(theta-offset)), 0.5f + (0.3f*sin(theta-offset)), 0.05f);
}
// Draw circle 
void circle(float x, float y, float r){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int n= 0; n <= 24; n++)
	{
		float const t = 2 * 3.14159*(float)n / 24.0f;
		glVertex2f(x + sin(t)*r, y + cos(t)*r);
	}
	glEnd();
}
// Plot attractor in x[],y[],num 
void plot_attractor(float x[], float y[], int n)
{
	float minx, maxx, miny, maxy;
	maxx = minx = x[0];
	maxy = miny = y[0];
	for (int j = 1; j < n; j++)
	{
		if (maxx<x[j])maxx = x[j];
		if (minx>x[j])minx = x[j];
		if (maxy<y[j])maxy = y[j];
		if (miny>y[j])miny = y[j];
	}

	gl_pnum(0, -0.08, minx);
	gl_pnum(0.9, -0.08, maxx);
	gl_pstring(0.4, -0.12, "Angle");
	gl_pnum(-0.24, 0, miny);
	gl_pnum(-0.24, 0.95, maxy);
	gl_pstring(-0.25, 0.5, "Velocity");

	gl_pstring(-0.1, -0.17, "Velocity=");
	gl_pnum(0.2, -0.17, v);

	gl_pstring(0.6, -0.17, "Angle=");
	gl_pnum(0.85, -0.17, a);



	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.01, -0.01);
	glVertex2f(-0.01, 1.01);
	glVertex2f(1.01, 1.01);
	glVertex2f(1.01, -0.01);
	glEnd();

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	for (int j = 0; j < n; j++){
		glVertex2f((x[j]-minx) / (maxx-minx), (y[j]-miny) / (maxy-miny));
	}
	glEnd();
}
// Print the value of a variable on screen 
void gl_pnum(float x, float y, float n){
	char text[20];
	sprintf_s(text, "%3.2f", n);
	gl_pstring(x, y, text);
}

// Print a string on screen 
void gl_pstring(float x, float y, char text[]){
	glPushMatrix();
	glTranslatef(x, y, 1);
	glScalef(0.0005, 0.0005, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char *)text);
	glPopMatrix();
}

static void Draw(void){
	// Draw ball motion on left of screen
	glViewport(0, 0, 400, 400);

	// Make the following region active;
	glScissor(0, 0, 400, 400);
	glEnable(GL_SCISSOR_TEST);

	glClearColor(0, 0, 0, 1);

	// Clear left side
	glClear(GL_COLOR_BUFFER_BIT);

	// Set drawing colour to red (red, green, blue)
	glColor3f(1.0, 0.0, 0.0);

	//Draw the pendulum with angle a
	draw_bob(a);

	// Set screen back to normal
	glScissor(400, 0, 800, 400);
	

	glColor3f(0.0, 1.0, 0.0);
	
	glViewport(400, 0, 400, 400);
	
	glClearColor(1, 1, 1, 1);
	//Clear right side
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(4.0);

	plot_attractor(x, y, i);
	glDisable(GL_SCISSOR_TEST);

	// Send message to cause re-draw (to return to Draw)
	glutPostRedisplay();
	glutSwapBuffers();
}
static void Update(int dummy)
{
	// Advance pendulum using Euler method 
	x[i] = a;
	y[i] = v;
	a = a + (v*dt);
	v = v + ((((-g / l)*sin(a)) + (D*v) + (c*fabs(sin(b*t))))*dt);
	t = t + dt;
	if (i<NUM) i++;

	glutTimerFunc(FRAMETIMEMS, Update, 0); 
}
//Called when a key is pressed
static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'u': v += 5; break;
	case 'd': v -= 5; break;
	case 'l': a += 1; break;
	case 'r': a -= 1; break;
	case 'Q':
	case 'q': glutLeaveMainLoop(); break;
	default: break;
	}
	glutPostRedisplay();
}
// Program starts here
int main(int argc, char * argv[]){
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(800, 400);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	window1 = glutCreateWindow("CS426 Assignment 4 - Pendulum and Attractor");

	// Set clearscreen colour to white
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	
	gluOrtho2D(-0.3, 1.1, -0.2, 1.1);
	
	glutDisplayFunc(Draw);
	glutTimerFunc(FRAMETIMEMS, Update, 0); 
	glutKeyboardFunc(key);
	glutMainLoop();

	return 0;
}
