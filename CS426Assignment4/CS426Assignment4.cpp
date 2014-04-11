// C.Markham Feb 2010

// FreeGlut Version of code created in lecture (with an extra 'paddle')
// You could create your own binaries from the freeglut VS2008 project supplied from the download site.
// http://freeglut.sourceforge.net/
// In this example the "Solution" contains both our code and binaries created from the above and copied into this project.
// The binaries are the include files, library files and dynamic linked libaries from the freeglut code (without the project files or sourcecode).
// Select Build - Clean Solution - Build - Rebuild Solution and then click on Start Debugging (run) and it should work.
// You may need to right click on First_FreeGlut on Solution explorer (see panel to left of this) and select Properties (last on list)
// Then choose the Configuration (Top left) All Configuarions then
// Configuration Properties - C\C++ - General - Additional Include Directories then
// either edit the path to the following or double click in the white space to the right of the "Additional Include Directories" 
// to create a new directory that points to the binaries the path will be something like the following but not the same
// "C:\Documents and Settings\ ...specific to your machine ...\My Documents\Visual Studio 2008\Projects\First_FreeGlut\freeglut_bin"
// Repeat the procedure for the Linker so the complier can find the freeglut.lib file in the same directory
// Configuration Properties - Linker - General - Additional Library Directories (a bit further down the list)
// Finally put freeglut.dll in the runtime directory, to cover both Release and Debug modes you will need to do this twice (Debug will do)
// C:\Documents and Settings\...specific to your machine...\My Documents\Visual Studio 2008\Projects\First_FreeGlut\Release
// C:\Documents and Settings\...specific to your machine...\My Documents\Visual Studio 2008\Projects\First_FreeGlut\Debug
// If all else fails you could try double clicking on First_FreeGlut in the release directory and see it running.
// VS2010 Right click on First_FreeGlut in Solution explorer (then VC++ directories) and add freeglut library to both Include Linker directories

#include "stdafx.h"

#include <freeglut.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// Black box moving left/right position
float xb = 200.0f;
float v = 0.0f;
float t = 0.0f;
float dt = 0.10f;

float yb = -160.0f;

// Blue paddle moving up/down
int yp = 0;

/* GLUT callback Handlers */
static void display(void)
{
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Set drawing colour to black (red, green, blue)
	glColor3f(0.0, 0.0, 0.0);
	// Draw a 20x20 rectangle centered on (x,0)
	glRectf(240 + yb, 240 + xb, 272 + yb, 272 + xb);

	// Set drawing colour to black (red, green, blue)
	glColor3f(0.0, 0.0, 1.0);

	// Draw a 20x20 rectangle centered on (x,0)
	glRecti(246, 210 + yp, 262, 312 + yp);


	glutSwapBuffers();
}


// Callback function to allow dynamic resizing of runtime display
static void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);

	// Maps 512x512 vertex co-ords 1:1 to the screen co-ords 
	// Rare in practice that you would do this
	gluOrtho2D(0.0, 512.0, 0.0, 512.0);
}


// Callback to manage standard keys on the keyboard
// In this case 'u' moves paddle up and 'd' moves paddle down
static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'u': yp += 5; break;
	case 'd': yp -= 5; break;

	case 'Q':
	case 'q': glutLeaveMainLoop(); break;  // (Stop!)

	default: break;
	}

	glutPostRedisplay();
}

// Callback to cursor keys on the keyboard
// In this case Up-Arrow moves paddle up and Down-Arrow moves paddle down

static void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:        yp += 5;        break;
	case GLUT_KEY_DOWN:      yp -= 5;        break;

	default: break;
	}
}


// Runs during idle time
// Double buffer this is the frame rate of the machine typically 50/60Hz
// Single buffer this is very much faster
static void idle(void)
{
	// Simulate the falling motion of the ball
	// Second order differential equation written as two first order
	// Evaluated with Euler/Cromer method
	xb = xb + (v*dt);
	v = v - (1.0f)*xb*dt;
	// Bounce added
	if (xb<0.0f) { v = -0.7f*v; xb = 0.0f; }

	t = t + dt;

	// Move horizonatal at fixed speed
	yb = yb + 0.3f;

	// Tell OpenGL to redraw screen now you have updated the 'game play'
	glutPostRedisplay();
}


/* Program entry point - starts here */
int main(int argc, char *argv[])
{
	// Create a Window called "FreeGlut One" at 100,100 on main screen, sized 512x512 pixels
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow("FreeGLUT One");

	// 2D program use an Orthographic projection to keep everything independent of Z
	glMatrixMode(GL_PROJECTION);
	// Maps 512x512 vertex co-ords 1:1 to the screen co-ords 
	// Rare in practice that you would do this
	gluOrtho2D(0.0, 512.0, 0.0, 512.0);


	// Set screen to yellow
	glClearColor(1, 1, 0, 1);  // Clearscree(Red, Green, Blue)

	// Load location of ReShapeFunction into mainloop so it can be called if required 
	// "resize" is both the name of the function and when referenced the memory address of the function 
	glutReshapeFunc(resize);

	// Callback function used when mainloop decides/graphics card to re-draw the scene
	// Sending a glutPostRedisplay(); message to mainloop will cause display to be called
	glutDisplayFunc(display);

	// Callback functions to manage keyboard input
	glutKeyboardFunc(key);
	glutSpecialFunc(special);

	// Called during idle time (like Update in XNA)
	// Doublebuffer at frame rate
	glutIdleFunc(idle);

	// Start mainloop running (go!)
	glutMainLoop();

	return EXIT_SUCCESS;
}
