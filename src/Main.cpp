/*
	Main.cpp
	Blaze Game Engine 0.03

	Created by Ned Bingham on 10/2/06.
	Copyright 2006 Sol Union. All rights reserved.

    Blaze Game Engine 0.03 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.03 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.03.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CoreGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"

CoreGraphics		Renderer;		// Handles all of the OpenGL rendering
Keyboard		KeyboardHandler;// Handles all keyboard input events
Mouse			MouseHandler;	// Handles all mouse input events

bool windowed = false;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, 1366, 768);
	Renderer.SetupPerspective();
	Renderer.Init();
	glLoadIdentity();
	// initialize anything you need to here

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clears the back buffer
	glutSwapBuffers();								// Switches the front and back buffers
}

void displayfunc()
{
	KeyboardHandler.HandleKeyStillDown();				// Checks for keys that are being held down

	Renderer.DrawFrame();

	glutSwapBuffers();							// Swaps the front and back buffers
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pmotionfunc(int x, int y)
{
	MouseHandler.HandleMouseMoved(x, y);
}

void mousefunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		MouseHandler.HandleMouseDown(button, x, y);
	else if (state == GLUT_UP)
		MouseHandler.HandleMouseUp(button, x, y);
}

void motionfunc(int x, int y)
{
	MouseHandler.HandleMouseMoved(x, y);
	MouseHandler.HandleMouseDown(0, x, y);
}

void keydownfunc(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);

	KeyboardHandler.HandleKeyDown(key);
}

void keyupfunc(unsigned char key, int x, int y)
{
	KeyboardHandler.HandleKeyUp(key);
}

void release()
{
	Renderer.Release();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	atexit(release);

	if (windowed)
	{
		glutInitWindowSize(1366, 768);
		glutInitWindowPosition(0, 0);
		glutCreateWindow("BGE");
	}
	else
	{
		glutGameModeString("1366x768:32@60");
		glutEnterGameMode();
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	if (GLEW_ARB_vertex_program)
		fprintf(stdout, "Status: ARB vertex programs available.\n");

	if (glewGetExtension("GL_ARB_fragment_program"))
		fprintf(stdout, "Status: ARB fragment programs available.\n");

	if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
		fprintf(stdout, "Status: ARB point sprites available.\n");

	glutReshapeFunc(reshape);
	glutDisplayFunc(displayfunc);
	glutIdleFunc(displayfunc);
	glutPassiveMotionFunc(pmotionfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutKeyboardFunc(keydownfunc);
	glutKeyboardUpFunc(keyupfunc);

	glutSetCursor(GLUT_CURSOR_NONE);

	init();
	glutMainLoop();
	if (!windowed)
		glutLeaveGameMode();
}

