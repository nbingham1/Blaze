#include "graphics.h"
#include "standard.h"

#include "CoreGraphics.h"

#include "keyboard.h"
#include "mouse.h"

CoreGraphics renderer;
keyboardhdl keys;
mousehdl mouse;

void initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	mouse.init(200.0, 900, 1600);
	renderer.Init();
}

void displayfunc()
{
	if (keys.keystates['w'])
		renderer.view.Move(Vector(0.0, 0.0, 0.001));
	if (keys.keystates['a'])
		renderer.view.Move(Vector(-0.001, 0.0, 0.0));
	if (keys.keystates['s'])
		renderer.view.Move(Vector(0.0, 0.0, -0.001));
	if (keys.keystates['d'])
		renderer.view.Move(Vector(0.001, 0.0, 0.0));
	if (keys.keystates['e'])
		renderer.view.Move(Vector(0.0, 0.001, 0.0));
	if (keys.keystates['q'])
		renderer.view.Move(Vector(0.0, -0.001, 0.0));
	if (keys.keystates['.'])
		renderer.view.move_mult *= 2.0;
	if (keys.keystates[','])
		renderer.view.move_mult *= .5;
	if (keys.keystates['z'])
		renderer.view.Host->Physics.LinearVelocity = Vector(0.0, 0.0, 0.0);

	renderer.RenderFrame();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void pmotionfunc(int x, int y)
{
	renderer.view.Rotate(mouse.getdelta(x, y));

	if (mouse.v > 900*3/4 || mouse.v < 900*1/4 || mouse.h > 1600*3/4 || mouse.h < 1600*1/4)
		mouse.setmouseloc(1600/2, 900/2);
}

void mousefunc(int button, int state, int x, int y)
{
}

void motionfunc(int x, int y)
{
	renderer.view.Rotate(mouse.getdelta(x, y));

	if (mouse.v > 900*3/4 || mouse.v < 900*1/4 || mouse.h > 1600*3/4 || mouse.h < 1600*1/4)
		mouse.setmouseloc(1600/2,900/2);
}

void keydownfunc(unsigned char key, int x, int y)
{
	if (key == 'u')
		exit(0);
	keys.keydown(key);
}

void keyupfunc(unsigned char key, int x, int y)
{
	keys.keyup(key);
}

void release()
{
	renderer.Release();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	atexit(release);
	glutGameModeString("1600x900@60");
	glutEnterGameMode();

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glutReshapeFunc(reshape);
	glutDisplayFunc(displayfunc);
	glutIdleFunc(displayfunc);
	glutPassiveMotionFunc(pmotionfunc);
	glutMotionFunc(motionfunc);
	glutMouseFunc(mousefunc);
	glutKeyboardFunc(keydownfunc);
	glutKeyboardUpFunc(keyupfunc);

	glutSetCursor(GLUT_CURSOR_NONE);

	initialize();
	glutMainLoop();
	glutLeaveGameMode();
}
