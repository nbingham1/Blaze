/*
	Main.cpp
	Blaze Game Engine 0.01

	Created by Ned Bingham on 8/6/05.
  	Copyright 2005 Sol Union. All rights reserved.

    Blaze Game Engine 0.01 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Blaze Game Engine 0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Blaze Game Engine 0.01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Main.h"
#include "GameApp.h"

GameApp App;
bool windowed = false;

void init()
{
	App.Init();
}

void displayfunc()
{
	App.Update();
}

void reshape(int w, int h)
{
	App.SetViewport(w, h);
}

void pmotionfunc(int x, int y)
{
	App.Input.HandleMouseMoved(x, y);
}

void mousefunc(int button, int state, int x, int y)
{
	App.Input.HandleMouseDown(button, state, x, y);
}

void motionfunc(int x, int y)
{
	App.Input.HandleMouseMoved(x, y);
}

void keydownfunc(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);

	App.Input.HandleKeyDown(key);
}

void keyupfunc(unsigned char key, int x, int y)
{
	App.Input.HandleKeyUp(key);
}

void release()
{
	App.CleanUpApp();
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

	if (glewGetExtension("GL_ARB_vertex_program"))
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
